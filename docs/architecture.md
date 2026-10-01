# Aether Architecture

Hardware/firmware -> Linux kernel/drivers -> Aether Core -> Aether services -> application runtimes -> desktop.

Aether Core owns lifecycle, IPC contracts, capability discovery, security boundaries, and system identity.

Hardware is capability-driven: Aether never assumes a laptop has a fingerprint reader, IR camera, dedicated GPU, touchscreen, or other optional device.

Authentication is mediated by one Identity/Authentication service. Providers can expose passwords, PINs, TPM credentials, fingerprints, face recognition, security keys, smart cards, and future methods.

Linux applications execute natively. Windows applications/games use a compatibility stack. Android applications use an Android runtime/container. All integrate with common application discovery, permissions, filesystem, graphics, audio, and notification services.

The production desktop shell is a later layer. Early development uses minimal diagnostics so system behavior can be validated independently of visual design.
