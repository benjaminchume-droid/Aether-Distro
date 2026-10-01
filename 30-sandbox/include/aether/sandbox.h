#ifndef AETHER_SANDBOX_H
#define AETHER_SANDBOX_H

#include <stddef.h>
#include <stdint.h>
#include <sys/types.h>
#include <aether/types.h>
#include <aether/aegis.h>

enum {
    AETHER_SANDBOX_NEW_MOUNT_NS   = 1u << 0,
    AETHER_SANDBOX_NEW_PID_NS     = 1u << 1,
    AETHER_SANDBOX_NEW_NET_NS     = 1u << 2,
    AETHER_SANDBOX_NEW_IPC_NS     = 1u << 3,
    AETHER_SANDBOX_NEW_UTS_NS     = 1u << 4,
    AETHER_SANDBOX_NEW_USER_NS    = 1u << 5,
    AETHER_SANDBOX_PRIVATE_MOUNTS = 1u << 6,
    AETHER_SANDBOX_NO_NEW_PRIVS   = 1u << 7,
    AETHER_SANDBOX_STRICT_SECCOMP = 1u << 8,
    AETHER_SANDBOX_LANDLOCK_RO    = 1u << 9,
    AETHER_SANDBOX_CGROUP_LIMITS  = 1u << 10
};

typedef struct {
    uint32_t flags;
    uint64_t memory_bytes;
    uint64_t cpu_seconds;
    uint64_t file_bytes;
    uint64_t process_count;
    const char *const *readonly_paths;
    size_t readonly_path_count;
    const char *cgroup_name;
} aether_sandbox_profile_t;

int aether_sandbox_linux_available(void);
int aether_sandbox_landlock_available(void);
int aether_sandbox_cgroupv2_available(void);
aether_status_t aether_sandbox_apply(const aether_sandbox_profile_t *profile);
aether_status_t aether_sandbox_spawn(const aether_sandbox_profile_t *profile,
                                     const char *path,char *const argv[],char *const envp[],
                                     pid_t *pid_out);
aether_status_t aether_sandbox_spawn_subject(aether_id_t subject,const aether_sandbox_profile_t *profile,
                                             const char *path,char *const argv[],char *const envp[],
                                             pid_t *pid_out);
#endif
