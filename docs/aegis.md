# Aegis

Aegis is a first-class Aether enforcement plane shared by Aegis Shield and Aegis Sandbox.

## Enforcement model

Aether services and application runtimes submit resource requests to Aegis. Aegis evaluates a policy against the requesting subject, resource class, and optional scope. The decision is allow, deny, or audit.

Aegis is deliberately policy-driven rather than a hard-coded blocklist. Domain filters, filesystem scopes, device access, permissions, and runtime restrictions become policy data and enforcement adapters.

## Architecture

Application/runtime -> Aegis policy engine -> enforcement adapter -> Aether service/kernel -> hardware/network.

Initial Phase 4 work provides the policy engine and system-service integration points. Later phases add real enforcement adapters:

- Phase 5: permission broker, sandbox, namespaces, resource controls, credential/security integration.
- Phase 6: update/recovery policy protection.
- Phase 7: application manifests and runtime policy attachment.
- Phase 8: compositor/media isolation and screen-capture policy.
- Phase 9: Linux/Windows/Android/Android TV runtime adapters.
- Phase 10: Shield/Sandbox user controls.
- Phase 12: hardening, audit tooling, compatibility testing.

The policy engine must remain general-purpose: no fixed ad domains, simulated permissions, or predetermined application paths.
