# Runtime Strategy

Aether presents one application model over multiple execution backends.

Native Linux applications use the host ABI. Windows applications and games use a Wine/Proton-based compatibility stack. Android applications use a containerized Android runtime.

The runtime registry is deliberately generic so future runtimes can be added without redesigning the desktop or application model.
