enum keys { KEY_ESCAPE };


int handle_key(enum keys key)
{
    switch(key) {
        case KEY_ESCAPE:
            return 1;
        default:
            return 0;
    }
}


int handle_mouse(void)
{
    return 0;
}
