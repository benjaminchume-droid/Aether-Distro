# Roadmap

## Phase 1 — Foundation
- [x] Architecture and subsystem boundaries
- [x] Core types and status/error model
- [x] Capability model and registry
- [x] Authentication contracts
- [x] Power contracts and initial policy
- [x] Service contracts
- [x] IPC endpoint registration and dispatch
- [x] Typed event bus
- [x] Identity validation
- [x] Foundation unit/integration tests
- [x] CI build/test coverage

## Phase 2 — Bootable Aether
- [x] Upstream Linux kernel integration
- [x] Kernel configuration/build pipeline
- [x] EFI-capable x86_64 kernel baseline
- [x] Initramfs assembly
- [x] Aether PID 1
- [x] Early filesystem mounts
- [x] Service process supervision
- [x] Dependency-aware service startup
- [x] Child reaping and shutdown
- [x] Early boot logging
- [x] Optional emergency shell in initramfs
- [x] QEMU direct-kernel boot runner
- [x] GRUB boot configuration
- [x] UEFI-capable ISO assembly path
- [x] QEMU ISO boot runner
- [x] Phase 2 service lifecycle tests
- [ ] Hardware-backed boot validation

## Phase 3 — Hardware
- [x] Linux sysfs device discovery
- [x] Device manager
- [x] CPU/GPU/display/audio/input/camera discovery
- [x] Storage/network/Bluetooth discovery
- [x] Battery/thermal/power state discovery
- [x] Firmware inventory
- [ ] Hardware-backed boot validation

## Phase 4 — System Services & Aegis Foundation
- [x] Storage service
- [x] Network service
- [x] Audio/video service facades
- [x] Session service
- [x] Notification/clipboard/search infrastructure
- [x] Aegis policy model and general-purpose rule engine
- [x] Aegis enforcement-plane architecture
- [x] System-service registry/integration contracts
- [x] Storage volume service facade
- [x] Network service facade
- [x] Audio service facade
- [x] Session state service
- [x] Notification service
- [x] Clipboard service
- [x] Search service
- [x] Aegis policy unit tests
- [ ] Aegis kernel/network/filesystem enforcement adapters

## Phase 5 — Security, Identity & Aegis Sandbox
- [x] Host identity lookup
- [x] Account enumeration and lookup
- [x] Authentication provider/challenge orchestration
- [x] TPM discovery and device availability
- [ ] Accounts lifecycle management
- [ ] Password/PIN authentication
- [ ] Security keys
- [x] Biometric provider registry
- [ ] Fingerprint provider implementation
- [ ] Face provider implementation
- [x] Kernel-backed cryptographic random, constant-time compare, and secure wipe primitives
- [x] Aegis-backed permission broker
- [x] Aegis policy attachment to identities/apps
- [x] Aegis policy inspection and persistence
- [ ] Aegis filesystem/network/device enforcement
- [x] Linux namespace/no_new_privs/resource-limit sandbox adapter
- [x] cgroup v2 memory/PID/CPU quota adapter
- [x] Landlock filesystem confinement adapter
- [ ] seccomp policy adapter
- [ ] Aegis virtualized filesystem/storage/resource providers
- [x] Aegis-gated sandbox process launch
- [ ] Full application sandboxing

## Phase 6 — Lifecycle
- [ ] Package manager
- [ ] Installer
- [ ] Atomic updates
- [ ] Rollback
- [ ] Recovery
- [ ] Safe mode

## Phase 7 — Application Platform
- [ ] Application registry
- [ ] Native Linux runtime
- [ ] Application manifests
- [ ] Runtime permissions/resources

## Phase 8 — Graphics & Desktop Infrastructure
- [ ] Wayland
- [ ] Compositor
- [ ] Window manager
- [ ] GPU acceleration
- [ ] Multi-monitor/HDR/VRR
- [ ] Video acceleration
- [ ] Accessibility

## Phase 9 — Compatibility
- [ ] Windows runtime
- [ ] Wine integration
- [ ] DXVK/VKD3D
- [ ] Proton/game integration
- [ ] Android runtime
- [ ] Android TV adapter

## Phase 10 — Aether Desktop
- [ ] Shell
- [ ] Launcher
- [ ] Dock/panel
- [ ] Settings
- [ ] File manager
- [ ] Login/lock UI
- [ ] Control Center

## Phase 11 — Developer Platform
- [ ] Aether SDK
- [ ] Application/service APIs
- [ ] Packaging/development tools
- [ ] Documentation

## Phase 12 — Release Engineering
- [ ] Diagnostics
- [ ] Hardware compatibility matrix
- [ ] Performance/power optimization
- [ ] Security hardening
- [ ] Regression suite
- [ ] Release images
