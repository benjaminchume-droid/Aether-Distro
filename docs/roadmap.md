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
- [ ] Storage service
- [ ] Network service
- [ ] Audio/video services
- [ ] Session service
- [ ] Notification/clipboard/search infrastructure
- [x] Aegis policy model and general-purpose rule engine
- [x] Aegis enforcement-plane architecture
- [x] System-service registry/integration contracts
- [x] Aegis policy unit tests
- [ ] Aegis kernel/network/filesystem enforcement adapters

## Phase 5 — Security, Identity & Aegis Sandbox
- [ ] Accounts
- [ ] Password/PIN authentication
- [ ] TPM
- [ ] Security keys
- [ ] Fingerprint provider
- [ ] Face provider
- [ ] Permission broker
- [ ] Aegis policy attachment to identities/apps
- [ ] Aegis filesystem/network/device enforcement
- [ ] Linux namespaces/cgroups/seccomp/Landlock sandbox adapters
- [ ] Aegis virtualized filesystem/storage/resource providers
- [ ] Sandboxing

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
