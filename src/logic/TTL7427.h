/* 3-INPUT NOR */

typedef unsigned char TTL7427_PINS;

#define TTL7427_A   (1 << 0)
#define TTL7427_B   (1 << 1)
#define TTL7427_C   (1 << 2)
#define TTL7427_Y   (1 << 3)
#define TTL7427_IN  (TTL7427_A|TTL7427_B|TTL7427_C)

#define TTL7427_INIT   0b00000000
#define TTL7427_ACT(p) (TTL7427[(p)&TTL7427_IN])

#define TTL7427_SET_A(p) ((p)|=TTL7427_A)
#define TTL7427_RESET_A(p) ((p)&=~TTL7427_A)
#define TTL7427_PUT_A(p,t) ((p)&=~TTL7427_A,(p)|=((t)?TTL7427_A:0))
#define TTL7427_SET_B(p) ((p)|=TTL7427_B)
#define TTL7427_RESET_B(p) ((p)&=~TTL7427_B)
#define TTL7427_PUT_B(p,t) ((p)&=~TTL7427_B,(p)|=((t)?TTL7427_B:0))
#define TTL7427_SET_C(p) ((p)|=TTL7427_C)
#define TTL7427_RESET_C(p) ((p)&=~TTL7427_C)
#define TTL7427_PUT_C(p,t) ((p)&=~TTL7427_C,(p)|=((t)?TTL7427_C:0))

#define TTL7427_GET_Y(p) ((p)&TTL7427_Y)

TTL7427_PINS TTL7427[] = {
    0b00001000,
    0b00000001,
    0b00000010,
    0b00000011,
    0b00000100,
    0b00000101,
    0b00000110,
    0b00000111,
};
