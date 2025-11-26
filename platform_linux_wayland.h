#ifndef PLATFORM_LINUX_WAYLAND_H
#define PLATFORM_LINUX_WAYLAND_H
#define _POSIX_C_SOURCE 200112L
#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <inttypes.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/un.h>
#include <unistd.h>

#define cstring_len(s) (sizeof(s) - 1)
#define roundup_4(n) (((n) + 3) & -4)

// Wayland protocol constants
static const uint32_t wayland_display_object_id = 1;
static const uint16_t wayland_wl_registry_event_global = 0;
static const uint16_t wayland_shm_pool_event_format = 0;
static const uint16_t wayland_wl_buffer_event_release = 0;
static const uint16_t wayland_xdg_wm_base_event_ping = 0;
static const uint16_t wayland_xdg_toplevel_event_configure = 0;
static const uint16_t wayland_xdg_toplevel_event_close = 1;
static const uint16_t wayland_xdg_surface_event_configure = 0;
static const uint16_t wayland_wl_display_get_registry_opcode = 1;
static const uint16_t wayland_wl_registry_bind_opcode = 0;
static const uint16_t wayland_wl_compositor_create_surface_opcode = 0;
static const uint16_t wayland_xdg_wm_base_pong_opcode = 3;
static const uint16_t wayland_xdg_surface_ack_configure_opcode = 4;
static const uint16_t wayland_wl_shm_create_pool_opcode = 0;
static const uint16_t wayland_xdg_wm_base_get_xdg_surface_opcode = 2;
static const uint16_t wayland_wl_shm_pool_create_buffer_opcode = 0;
static const uint16_t wayland_wl_surface_attach_opcode = 1;
static const uint16_t wayland_xdg_surface_get_toplevel_opcode = 1;
static const uint16_t wayland_wl_surface_commit_opcode = 6;
static const uint16_t wayland_wl_display_error_event = 0;
static const uint16_t wayland_xdg_toplevel_set_title_opcode = 2;
static const uint32_t wayland_format_xrgb8888 = 1;
static const uint32_t wayland_header_size = 8;
static const uint32_t color_channels = 4;

typedef enum {
    WINDOW_STATE_NONE,
    WINDOW_STATE_CREATED,
    WINDOW_STATE_CONFIGURED,
    WINDOW_STATE_READY,
    WINDOW_STATE_CLOSED
} window_state_t;

typedef struct pal_window {
    uint32_t id;
    uint32_t wl_registry;
    uint32_t wl_shm;
    uint32_t wl_shm_pool;
    uint32_t wl_buffer;
    uint32_t xdg_wm_base;
    uint32_t xdg_surface;
    uint32_t wl_compositor;
    uint32_t wl_surface;
    uint32_t xdg_toplevel;
    uint32_t stride;
    uint32_t width;
    uint32_t height;
    uint32_t shm_pool_size;
    int shm_fd;
    uint8_t *shm_pool_data;
    window_state_t state;
    int should_close;
} pal_window;

// Global state
static int g_wayland_fd = -1;
static uint32_t g_wayland_current_id = 1;
static pal_window **g_windows = NULL;
static size_t g_window_count = 0;
static size_t g_window_capacity = 0;

// Buffer utility functions
static void buf_write_u32(char *buf, uint64_t *buf_size, uint64_t buf_cap, uint32_t x) {
    assert(*buf_size + sizeof(x) <= buf_cap);
    assert(((size_t)buf + *buf_size) % sizeof(x) == 0);
    *(uint32_t *)(buf + *buf_size) = x;
    *buf_size += sizeof(x);
}

static void buf_write_u16(char *buf, uint64_t *buf_size, uint64_t buf_cap, uint16_t x) {
    assert(*buf_size + sizeof(x) <= buf_cap);
    assert(((size_t)buf + *buf_size) % sizeof(x) == 0);
    *(uint16_t *)(buf + *buf_size) = x;
    *buf_size += sizeof(x);
}

static void buf_write_string(char *buf, uint64_t *buf_size, uint64_t buf_cap, const char *src, uint32_t src_len) {
    assert(*buf_size + src_len <= buf_cap);
    buf_write_u32(buf, buf_size, buf_cap, src_len);
    memcpy(buf + *buf_size, src, src_len);
    uint32_t padded = roundup_4(src_len);
    if (padded > src_len) {
        memset(buf + *buf_size + src_len, 0, padded - src_len);
    }
    *buf_size += padded;
}

static uint32_t buf_read_u32(char **buf, uint64_t *buf_size) {
    assert(*buf_size >= sizeof(uint32_t));
    assert((size_t)*buf % sizeof(uint32_t) == 0);
    uint32_t res = *(uint32_t *)(*buf);
    *buf += sizeof(res);
    *buf_size -= sizeof(res);
    return res;
}

static uint16_t buf_read_u16(char **buf, uint64_t *buf_size) {
    assert(*buf_size >= sizeof(uint16_t));
    assert((size_t)*buf % sizeof(uint16_t) == 0);
    uint16_t res = *(uint16_t *)(*buf);
    *buf += sizeof(res);
    *buf_size -= sizeof(res);
    return res;
}

static void buf_read_n(char **buf, uint64_t *buf_size, char *dst, uint64_t n) {
    assert(*buf_size >= n);
    memcpy(dst, *buf, n);
    *buf += n;
    *buf_size -= n;
}

// Window registry functions
static void register_window(pal_window *window) {
    if (g_window_count >= g_window_capacity) {
        size_t new_capacity = g_window_capacity == 0 ? 4 : g_window_capacity * 2;
        pal_window **new_windows = (pal_window**)realloc(g_windows, new_capacity * sizeof(pal_window *));
        if (new_windows) {
            g_windows = new_windows;
            g_window_capacity = new_capacity;
        }
    }
    
    if (g_window_count < g_window_capacity) {
        g_windows[g_window_count++] = window;
    }
}

static void unregister_window(pal_window *window) {
    for (size_t i = 0; i < g_window_count; i++) {
        if (g_windows[i] == window) {
            g_windows[i] = g_windows[g_window_count - 1];
            g_window_count--;
            break;
        }
    }
}

static pal_window *find_window_by_object_id(uint32_t object_id) {
    for (size_t i = 0; i < g_window_count; i++) {
        pal_window *w = g_windows[i];
        if (w->wl_surface == object_id || w->xdg_surface == object_id ||
            w->xdg_toplevel == object_id || w->wl_buffer == object_id ||
            w->xdg_wm_base == object_id || w->wl_registry == object_id ||
            w->wl_shm == object_id) {
            return w;
        }
    }
    return NULL;
}

// Wayland protocol functions
static int wayland_display_connect() {
    char *xdg_runtime_dir = getenv("XDG_RUNTIME_DIR");
    if (xdg_runtime_dir == NULL) {
        return -1;
    }

    uint64_t xdg_runtime_dir_len = strlen(xdg_runtime_dir);
    struct sockaddr_un addr = {.sun_family = AF_UNIX};
    
    if (xdg_runtime_dir_len > sizeof(addr.sun_path) - 20) {
        return -1;
    }

    uint64_t socket_path_len = 0;
    memcpy(addr.sun_path, xdg_runtime_dir, xdg_runtime_dir_len);
    socket_path_len += xdg_runtime_dir_len;
    addr.sun_path[socket_path_len++] = '/';

    char *wayland_display = getenv("WAYLAND_DISPLAY");
    if (wayland_display == NULL) {
        char wayland_display_default[] = "wayland-0";
        uint64_t wayland_display_default_len = cstring_len(wayland_display_default);
        memcpy(addr.sun_path + socket_path_len, wayland_display_default, wayland_display_default_len);
        socket_path_len += wayland_display_default_len;
    } else {
        uint64_t wayland_display_len = strlen(wayland_display);
        memcpy(addr.sun_path + socket_path_len, wayland_display, wayland_display_len);
        socket_path_len += wayland_display_len;
    }

    int fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (fd == -1) {
        return -1;
    }

    if (connect(fd, (struct sockaddr *)&addr, sizeof(addr)) == -1) {
        close(fd);
        return -1;
    }

    return fd;
}

static uint32_t wayland_wl_display_get_registry(int fd) {
    uint64_t msg_size = 0;
    char msg[128] = "";
    buf_write_u32(msg, &msg_size, sizeof(msg), wayland_display_object_id);
    buf_write_u16(msg, &msg_size, sizeof(msg), wayland_wl_display_get_registry_opcode);
    
    uint16_t msg_announced_size = wayland_header_size + sizeof(g_wayland_current_id);
    buf_write_u16(msg, &msg_size, sizeof(msg), msg_announced_size);
    
    g_wayland_current_id++;
    buf_write_u32(msg, &msg_size, sizeof(msg), g_wayland_current_id);

    if ((int64_t)msg_size != send(fd, msg, msg_size, 0)) {
        return 0;
    }

    return g_wayland_current_id;
}

static uint32_t wayland_wl_registry_bind(int fd, uint32_t registry, uint32_t name, const char *interface, uint32_t interface_len, uint32_t version) {
    uint64_t msg_size = 0;
    char msg[512] = "";
    buf_write_u32(msg, &msg_size, sizeof(msg), registry);
    buf_write_u16(msg, &msg_size, sizeof(msg), wayland_wl_registry_bind_opcode);

    uint16_t msg_announced_size = wayland_header_size + sizeof(name) + sizeof(interface_len) +
                                  roundup_4(interface_len) + sizeof(version) + sizeof(g_wayland_current_id);
    buf_write_u16(msg, &msg_size, sizeof(msg), msg_announced_size);

    buf_write_u32(msg, &msg_size, sizeof(msg), name);
    buf_write_string(msg, &msg_size, sizeof(msg), interface, interface_len);
    buf_write_u32(msg, &msg_size, sizeof(msg), version);

    g_wayland_current_id++;
    buf_write_u32(msg, &msg_size, sizeof(msg), g_wayland_current_id);

    if ((int64_t)msg_size != send(fd, msg, msg_size, 0)) {
        return 0;
    }

    return g_wayland_current_id;
}

static uint32_t wayland_wl_compositor_create_surface(int fd, uint32_t compositor) {
    uint64_t msg_size = 0;
    char msg[128] = "";
    buf_write_u32(msg, &msg_size, sizeof(msg), compositor);
    buf_write_u16(msg, &msg_size, sizeof(msg), wayland_wl_compositor_create_surface_opcode);

    uint16_t msg_announced_size = wayland_header_size + sizeof(g_wayland_current_id);
    buf_write_u16(msg, &msg_size, sizeof(msg), msg_announced_size);

    g_wayland_current_id++;
    buf_write_u32(msg, &msg_size, sizeof(msg), g_wayland_current_id);

    if ((int64_t)msg_size != send(fd, msg, msg_size, 0)) {
        return 0;
    }

    return g_wayland_current_id;
}

static void create_shared_memory_file(uint64_t size, pal_window *window) {
    char name[256] = "/pal-shm-";
    for (uint64_t i = 9; i < 255; i++) {
        name[i] = ((double)rand()) / (double)RAND_MAX * 26 + 'a';
    }
    name[255] = '\0';

    int fd = shm_open(name, O_RDWR | O_EXCL | O_CREAT, 0600);
    if (fd == -1) {
        return;
    }

    shm_unlink(name);

    if (ftruncate(fd, size) == -1) {
        close(fd);
        return;
    }

    window->shm_pool_data = (uint8_t*)mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (window->shm_pool_data == MAP_FAILED) {
        close(fd);
        window->shm_pool_data = NULL;
        return;
    }
    
    window->shm_fd = fd;
}

static void wayland_xdg_wm_base_pong(int fd, uint32_t xdg_wm_base, uint32_t ping) {
    uint64_t msg_size = 0;
    char msg[128] = "";
    buf_write_u32(msg, &msg_size, sizeof(msg), xdg_wm_base);
    buf_write_u16(msg, &msg_size, sizeof(msg), wayland_xdg_wm_base_pong_opcode);

    uint16_t msg_announced_size = wayland_header_size + sizeof(ping);
    buf_write_u16(msg, &msg_size, sizeof(msg), msg_announced_size);
    buf_write_u32(msg, &msg_size, sizeof(msg), ping);

    send(fd, msg, msg_size, 0);
}

static void wayland_xdg_surface_ack_configure(int fd, uint32_t xdg_surface, uint32_t configure) {
    uint64_t msg_size = 0;
    char msg[128] = "";
    buf_write_u32(msg, &msg_size, sizeof(msg), xdg_surface);
    buf_write_u16(msg, &msg_size, sizeof(msg), wayland_xdg_surface_ack_configure_opcode);

    uint16_t msg_announced_size = wayland_header_size + sizeof(configure);
    buf_write_u16(msg, &msg_size, sizeof(msg), msg_announced_size);
    buf_write_u32(msg, &msg_size, sizeof(msg), configure);

    send(fd, msg, msg_size, 0);
}

static uint32_t wayland_wl_shm_create_pool(int fd, pal_window *window) {
    uint64_t msg_size = 0;
    char msg[128] = "";
    buf_write_u32(msg, &msg_size, sizeof(msg), window->wl_shm);
    buf_write_u16(msg, &msg_size, sizeof(msg), wayland_wl_shm_create_pool_opcode);

    uint16_t msg_announced_size = wayland_header_size + sizeof(g_wayland_current_id) + sizeof(window->shm_pool_size);
    buf_write_u16(msg, &msg_size, sizeof(msg), msg_announced_size);

    g_wayland_current_id++;
    buf_write_u32(msg, &msg_size, sizeof(msg), g_wayland_current_id);
    buf_write_u32(msg, &msg_size, sizeof(msg), window->shm_pool_size);

    char buf[CMSG_SPACE(sizeof(window->shm_fd))];
    memset(buf, 0, sizeof(buf));

    struct iovec io = {.iov_base = msg, .iov_len = msg_size};
    struct msghdr socket_msg = {
        .msg_iov = &io,
        .msg_iovlen = 1,
        .msg_control = buf,
        .msg_controllen = sizeof(buf),
    };

    struct cmsghdr *cmsg = CMSG_FIRSTHDR(&socket_msg);
    cmsg->cmsg_level = SOL_SOCKET;
    cmsg->cmsg_type = SCM_RIGHTS;
    cmsg->cmsg_len = CMSG_LEN(sizeof(window->shm_fd));
    *((int *)CMSG_DATA(cmsg)) = window->shm_fd;

    if (sendmsg(fd, &socket_msg, 0) == -1) {
        return 0;
    }

    return g_wayland_current_id;
}

static uint32_t wayland_xdg_wm_base_get_xdg_surface(int fd, uint32_t xdg_wm_base, uint32_t wl_surface) {
    uint64_t msg_size = 0;
    char msg[128] = "";
    buf_write_u32(msg, &msg_size, sizeof(msg), xdg_wm_base);
    buf_write_u16(msg, &msg_size, sizeof(msg), wayland_xdg_wm_base_get_xdg_surface_opcode);

    uint16_t msg_announced_size = wayland_header_size + sizeof(g_wayland_current_id) + sizeof(wl_surface);
    buf_write_u16(msg, &msg_size, sizeof(msg), msg_announced_size);

    g_wayland_current_id++;
    buf_write_u32(msg, &msg_size, sizeof(msg), g_wayland_current_id);
    buf_write_u32(msg, &msg_size, sizeof(msg), wl_surface);

    if ((int64_t)msg_size != send(fd, msg, msg_size, 0)) {
        return 0;
    }

    return g_wayland_current_id;
}

static uint32_t wayland_wl_shm_pool_create_buffer(int fd, pal_window *window) {
    uint64_t msg_size = 0;
    char msg[128] = "";
    buf_write_u32(msg, &msg_size, sizeof(msg), window->wl_shm_pool);
    buf_write_u16(msg, &msg_size, sizeof(msg), wayland_wl_shm_pool_create_buffer_opcode);

    uint16_t msg_announced_size = wayland_header_size + sizeof(g_wayland_current_id) + sizeof(uint32_t) * 5;
    buf_write_u16(msg, &msg_size, sizeof(msg), msg_announced_size);

    g_wayland_current_id++;
    buf_write_u32(msg, &msg_size, sizeof(msg), g_wayland_current_id);

    uint32_t offset = 0;
    buf_write_u32(msg, &msg_size, sizeof(msg), offset);
    buf_write_u32(msg, &msg_size, sizeof(msg), window->width);
    buf_write_u32(msg, &msg_size, sizeof(msg), window->height);
    buf_write_u32(msg, &msg_size, sizeof(msg), window->stride);
    buf_write_u32(msg, &msg_size, sizeof(msg), wayland_format_xrgb8888);

    if ((int64_t)msg_size != send(fd, msg, msg_size, 0)) {
        return 0;
    }

    return g_wayland_current_id;
}

static void wayland_wl_surface_attach(int fd, uint32_t wl_surface, uint32_t wl_buffer) {
    uint64_t msg_size = 0;
    char msg[128] = "";
    buf_write_u32(msg, &msg_size, sizeof(msg), wl_surface);
    buf_write_u16(msg, &msg_size, sizeof(msg), wayland_wl_surface_attach_opcode);

    uint16_t msg_announced_size = wayland_header_size + sizeof(wl_buffer) + sizeof(uint32_t) * 2;
    buf_write_u16(msg, &msg_size, sizeof(msg), msg_announced_size);
    buf_write_u32(msg, &msg_size, sizeof(msg), wl_buffer);

    uint32_t x = 0, y = 0;
    buf_write_u32(msg, &msg_size, sizeof(msg), x);
    buf_write_u32(msg, &msg_size, sizeof(msg), y);

    send(fd, msg, msg_size, 0);
}

static uint32_t wayland_xdg_surface_get_toplevel(int fd, uint32_t xdg_surface) {
    uint64_t msg_size = 0;
    char msg[128] = "";
    buf_write_u32(msg, &msg_size, sizeof(msg), xdg_surface);
    buf_write_u16(msg, &msg_size, sizeof(msg), wayland_xdg_surface_get_toplevel_opcode);

    uint16_t msg_announced_size = wayland_header_size + sizeof(g_wayland_current_id);
    buf_write_u16(msg, &msg_size, sizeof(msg), msg_announced_size);

    g_wayland_current_id++;
    buf_write_u32(msg, &msg_size, sizeof(msg), g_wayland_current_id);

    if ((int64_t)msg_size != send(fd, msg, msg_size, 0)) {
        return 0;
    }

    return g_wayland_current_id;
}

static void wayland_xdg_toplevel_set_title(int fd, uint32_t xdg_toplevel, const char *title) {
    uint32_t title_len = strlen(title) + 1; // Include null terminator
    
    uint64_t msg_size = 0;
    char msg[512] = "";
    buf_write_u32(msg, &msg_size, sizeof(msg), xdg_toplevel);
    buf_write_u16(msg, &msg_size, sizeof(msg), wayland_xdg_toplevel_set_title_opcode);

    uint16_t msg_announced_size = wayland_header_size + sizeof(title_len) + roundup_4(title_len);
    buf_write_u16(msg, &msg_size, sizeof(msg), msg_announced_size);
    buf_write_string(msg, &msg_size, sizeof(msg), title, title_len);

    send(fd, msg, msg_size, 0);
}

static void wayland_wl_surface_commit(int fd, uint32_t wl_surface) {
    uint64_t msg_size = 0;
    char msg[128] = "";
    buf_write_u32(msg, &msg_size, sizeof(msg), wl_surface);
    buf_write_u16(msg, &msg_size, sizeof(msg), wayland_wl_surface_commit_opcode);

    uint16_t msg_announced_size = wayland_header_size;
    buf_write_u16(msg, &msg_size, sizeof(msg), msg_announced_size);

    send(fd, msg, msg_size, 0);
}

static void wayland_handle_message(int fd, pal_window *window, char **msg, uint64_t *msg_len) {
    if (*msg_len < 8) {
        return;
    }

    uint32_t object_id = buf_read_u32(msg, msg_len);
    uint16_t opcode = buf_read_u16(msg, msg_len);
    uint16_t announced_size = buf_read_u16(msg, msg_len);

    if (object_id == window->wl_registry && opcode == wayland_wl_registry_event_global) {
        uint32_t name = buf_read_u32(msg, msg_len);
        uint32_t interface_len = buf_read_u32(msg, msg_len);
        uint32_t padded_interface_len = roundup_4(interface_len);

        char interface[512] = "";
        buf_read_n(msg, msg_len, interface, padded_interface_len);
        uint32_t version = buf_read_u32(msg, msg_len);

        if (strcmp("wl_shm", interface) == 0) {
            window->wl_shm = wayland_wl_registry_bind(fd, window->wl_registry, name, interface, interface_len, version);
        } else if (strcmp("xdg_wm_base", interface) == 0) {
            window->xdg_wm_base = wayland_wl_registry_bind(fd, window->wl_registry, name, interface, interface_len, version);
        } else if (strcmp("wl_compositor", interface) == 0) {
            window->wl_compositor = wayland_wl_registry_bind(fd, window->wl_registry, name, interface, interface_len, version);
        }
    } else if (object_id == wayland_display_object_id && opcode == wayland_wl_display_error_event) {
        uint32_t target_object_id = buf_read_u32(msg, msg_len);
        uint32_t code = buf_read_u32(msg, msg_len);
        uint32_t error_len = buf_read_u32(msg, msg_len);
        char error[512] = "";
        buf_read_n(msg, msg_len, error, roundup_4(error_len));
        fprintf(stderr, "Wayland error: object=%u code=%u error=%s\n", target_object_id, code, error);
    } else if (object_id == window->wl_shm && opcode == wayland_shm_pool_event_format) {
        buf_read_u32(msg, msg_len); // format
    } else if (object_id == window->wl_buffer && opcode == wayland_wl_buffer_event_release) {
        // Buffer released
    } else if (object_id == window->xdg_wm_base && opcode == wayland_xdg_wm_base_event_ping) {
        uint32_t ping = buf_read_u32(msg, msg_len);
        wayland_xdg_wm_base_pong(fd, window->xdg_wm_base, ping);
    } else if (object_id == window->xdg_toplevel && opcode == wayland_xdg_toplevel_event_configure) {
        uint32_t w = buf_read_u32(msg, msg_len);
        uint32_t h = buf_read_u32(msg, msg_len);
        uint32_t len = buf_read_u32(msg, msg_len);
        char buf[256] = "";
        if (len <= sizeof(buf)) {
            buf_read_n(msg, msg_len, buf, len);
        }
        
        // Queue resize event if dimensions changed
        if ((w > 0 && w != window->width) || (h > 0 && h != window->height)) {
            pal_event event = {0};
            event.type = PAL_EVENT_WINDOW_RESIZED;
            event.window.windowid = window->id;
            event.window.width = w > 0 ? w : window->width;
            event.window.height = h > 0 ? h : window->height;
            pal__eventq_push(&g_event_queue, event);
        }
    } else if (object_id == window->xdg_surface && opcode == wayland_xdg_surface_event_configure) {
        uint32_t configure = buf_read_u32(msg, msg_len);
        wayland_xdg_surface_ack_configure(fd, window->xdg_surface, configure);
        window->state = WINDOW_STATE_CONFIGURED;
    } else if (object_id == window->xdg_toplevel && opcode == wayland_xdg_toplevel_event_close) {
        window->should_close = 1;
        
        // Queue close event
        pal_event event = {0};
        event.type = PAL_EVENT_WINDOW_CLOSE_REQUESTED;
        event.window.windowid = window->id;
        pal__eventq_push(&g_event_queue, event);
    }
}

static void process_events(pal_window *window) {
    char read_buf[4096] = "";
    int64_t read_bytes = recv(g_wayland_fd, read_buf, sizeof(read_buf), MSG_DONTWAIT);
    
    if (read_bytes <= 0) {
        return;
    }

    char *msg = read_buf;
    uint64_t msg_len = (uint64_t)read_bytes;

    while (msg_len > 0) {
        wayland_handle_message(g_wayland_fd, window, &msg, &msg_len);
    }
}

static void process_all_events() {
    char read_buf[4096] = "";
    int64_t read_bytes = recv(g_wayland_fd, read_buf, sizeof(read_buf), MSG_DONTWAIT);
    
    if (read_bytes <= 0) {
        return;
    }

    char *msg = read_buf;
    uint64_t msg_len = (uint64_t)read_bytes;

    while (msg_len >= 8) {
        // Peek at object_id to find the right window
        uint32_t object_id = *(uint32_t *)msg;
        pal_window *window = find_window_by_object_id(object_id);
        
        if (!window && g_window_count > 0) {
            window = g_windows[0]; // Fallback to first window
        }
        
        if (window) {
            wayland_handle_message(g_wayland_fd, window, &msg, &msg_len);
        } else {
            break; // Can't process without a window
        }
    }
}

// Public API Implementation

void pal_init() {
    struct timeval tv = {0};
    gettimeofday(&tv, NULL);
    srand(tv.tv_sec * 1000000 + tv.tv_usec);

    g_wayland_fd = wayland_display_connect();
    if (g_wayland_fd == -1) {
        fprintf(stderr, "Failed to connect to Wayland display\n");
        exit(1);
    }
    
    if (!pal__init_eventq()) {
        fprintf(stderr, "Failed to initialize event queue\n");
        exit(1);
    }
}

void pal_shutdown() {
    pal__eventq_free(g_event_queue);
    
    if (g_windows) {
        free(g_windows);
        g_windows = NULL;
    }
    g_window_count = 0;
    g_window_capacity = 0;
    
    if (g_wayland_fd >= 0) {
        close(g_wayland_fd);
        g_wayland_fd = -1;
    }
}

PALAPI pal_window *pal_create_window(int width, int height, const char *windowTitle, uint64_t window_flags) {
    (void)window_flags; // Unused for now

    if (g_wayland_fd == -1) {
        fprintf(stderr, "pal_init() must be called before creating windows\n");
        return NULL;
    }

    pal_window *window = calloc(1, sizeof(pal_window));
    if (!window) {
        return NULL;
    }

    static uint32_t next_window_id = 1;
    window->id = next_window_id++;
    window->width = width;
    window->height = height;
    window->stride = width * color_channels;
    window->shm_pool_size = window->height * window->stride;
    window->state = WINDOW_STATE_NONE;
    window->should_close = 0;

    // Register window early so it can receive events
    register_window(window);

    // Get registry
    window->wl_registry = wayland_wl_display_get_registry(g_wayland_fd);
    if (window->wl_registry == 0) {
        unregister_window(window);
        free(window);
        return NULL;
    }

    // Wait for registry events to bind interfaces
    int attempts = 0;
    while ((window->wl_compositor == 0 || window->wl_shm == 0 || window->xdg_wm_base == 0) && attempts < 100) {
        process_events(window);
        attempts++;
        sleep(1000); // 1ms
    }

    if (window->wl_compositor == 0 || window->wl_shm == 0 || window->xdg_wm_base == 0) {
        fprintf(stderr, "Failed to bind required Wayland interfaces\n");
        unregister_window(window);
        free(window);
        return NULL;
    }

    // Create shared memory
    create_shared_memory_file(window->shm_pool_size, window);
    if (window->shm_pool_data == NULL) {
        fprintf(stderr, "Failed to create shared memory\n");
        unregister_window(window);
        free(window);
        return NULL;
    }

    // Create surface and configure window
    window->wl_surface = wayland_wl_compositor_create_surface(g_wayland_fd, window->wl_compositor);
    if (window->wl_surface == 0) {
        munmap(window->shm_pool_data, window->shm_pool_size);
        close(window->shm_fd);
        unregister_window(window);
        free(window);
        return NULL;
    }

    window->xdg_surface = wayland_xdg_wm_base_get_xdg_surface(g_wayland_fd, window->xdg_wm_base, window->wl_surface);
    window->xdg_toplevel = wayland_xdg_surface_get_toplevel(g_wayland_fd, window->xdg_surface);
    
    if (windowTitle) {
        wayland_xdg_toplevel_set_title(g_wayland_fd, window->xdg_toplevel, windowTitle);
    }
    
    wayland_wl_surface_commit(g_wayland_fd, window->wl_surface);
    
    window->state = WINDOW_STATE_CREATED;

    // Wait for configure event
    attempts = 0;
    while (window->state != WINDOW_STATE_CONFIGURED && attempts < 100) {
        process_events(window);
        attempts++;
        usleep(1000);
    }

    if (window->state == WINDOW_STATE_CONFIGURED) {
        // Create buffer pool
        window->wl_shm_pool = wayland_wl_shm_create_pool(g_wayland_fd, window);
        window->wl_buffer = wayland_wl_shm_pool_create_buffer(g_wayland_fd, window);
        
        // Clear to black initially
        memset(window->shm_pool_data, 0, window->shm_pool_size);
        
        wayland_wl_surface_attach(g_wayland_fd, window->wl_surface, window->wl_buffer);
        wayland_wl_surface_commit(g_wayland_fd, window->wl_surface);
        
        window->state = WINDOW_STATE_READY;
        
        // Queue window shown event
        pal_event event = {0};
        event.type = PAL_EVENT_WINDOW_SHOWN;
        event.window.windowid = window->id;
        event.window.width = window->width;
        event.window.height = window->height;
        pal__eventq_push(&g_event_queue, event);
    }

    return window;
}

PALAPI void pal_close_window(pal_window *window) {
    if (!window) {
        return;
    }

    // Queue window closed event
    pal_event event = {0};
    event.type = PAL_EVENT_WINDOW_CLOSED;
    event.window.windowid = window->id;
    pal__eventq_push(&g_event_queue, event);

    unregister_window(window);

    if (window->shm_pool_data) {
        munmap(window->shm_pool_data, window->shm_pool_size);
    }
    
    if (window->shm_fd >= 0) {
        close(window->shm_fd);
    }

    window->state = WINDOW_STATE_CLOSED;
    free(window);
}

PALAPI pal_bool pal_poll_events(pal_event *event) {
    if (!event) {
        return pal_false;
    }

    // Process any pending Wayland events first
    process_all_events();

    // Check if queue has events
    if (g_event_queue.size == 0) {
        return pal_false;
    }

    // Peek
    *event = g_event_queue.events[g_event_queue.front];
    // Dequeue
    g_event_queue.front = (g_event_queue.front + 1) % g_event_queue.capacity;
    g_event_queue.size--;

    return pal_true;
}
#endif /* PLATFORM_LINUX_WAYLAND_H */
