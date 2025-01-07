/* 2-INPUT NOR */

typedef unsigned char TTL7402_PINS;

#define TTL7402_A   (1 << 0)
#define TTL7402_B   (1 << 1)
#define TTL7402_Y   (1 << 2)
#define TTL7402_IN  (TTL7402_A|TTL7402_B)

#define TTL7402_INIT   TTL7402_Y
#define TTL7402_ACT(p) (TTL7402[(p)&TTL7402_IN])

#define TTL7402_SET_A(p) ((p)|=TTL7402_A)
#define TTL7402_RESET_A(p) ((p)&=~TTL7402_A)
#define TTL7402_PUT_A(p,t) ((p)&=~TTL7402_A,(p)|=((t)?TTL7402_A:0))
#define TTL7402_SET_B(p) ((p)|=TTL7402_B)
#define TTL7402_RESET_B(p) ((p)&=~TTL7402_B)
#define TTL7402_PUT_B(p,t) ((p)&=~TTL7402_B,(p)|=((t)?TTL7402_B:0))

#define TTL7402_GET_Y(p) ((p)&TTL7402_Y)

TTL7402_PINS TTL7402[] = {
    0b00000100,
    0b00000001,
    0b00000010,
    0b00000011,
};
