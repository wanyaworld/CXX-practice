#include <fcntl.h>
#include <sys/fanotify.h>

int main() {
        int fd = fanotify_init (FAN_CLOEXEC,
                            O_RDONLY | O_CLOEXEC | O_LARGEFILE);
        return 0;
}
