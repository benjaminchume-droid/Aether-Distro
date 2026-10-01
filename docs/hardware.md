# Hardware Strategy

Aether uses capability discovery rather than laptop-specific assumptions.

The hardware layer will normalize Linux device information into Aether capabilities. Optional features such as fingerprint, face authentication, TPM, dedicated GPU, touchscreen, HDR, VRR, and battery telemetry appear only when the hardware exposes usable support.

A missing optional device is a normal state, not an error.
