#ifndef HANDLERS_H
#define HANDLERS_H


void draw(const struct bar * b);
void focus_editor(void);
void (*handle_keyboard)(struct keyboard * k);
void handle_mouse(struct mouse * m);


#endif  /* HANDLERS_H */
