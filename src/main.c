#include "includes/app.h"

int main(void)
{
    App_t *app = create_app();
    run_app(app);
    destroy_app(&app);
    return 0;
}
