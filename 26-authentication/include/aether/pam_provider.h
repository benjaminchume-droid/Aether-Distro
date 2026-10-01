#ifndef AETHER_PAM_PROVIDER_H
#define AETHER_PAM_PROVIDER_H

#include <aether/types.h>

aether_status_t aether_pam_password_provider_register(const char *service_name);
void aether_pam_password_provider_shutdown(void);
int aether_pam_password_provider_available(void);
#endif
