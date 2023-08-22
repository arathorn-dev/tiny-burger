#include "includes/app.h"

int main(void)
{
    App_t *app = create_app();
    if (app == NULL)
    {
        return 1;
    }
    run_app(app);
    destroy_app(&app);
    return 0;
}
