/* 4-INPUT NOR */

typedef unsigned char TTL7425_PINS;

#define TTL7425_A   (1 << 0)
#define TTL7425_B   (1 << 1)
#define TTL7425_C   (1 << 2)
#define TTL7425_D   (1 << 3)
#define TTL7425_Y   (1 << 4)
#define TTL7425_IN  (TTL7425_A|TTL7425_B|TTL7425_C|TTL7425_D)

#define TTL7425_INIT   TTL7425_Y
#define TTL7425_ACT(p) (TTL7425[(p)&TTL7425_IN])

#define TTL7425_SET_A(p) ((p)|=TTL7425_A)
#define TTL7425_RESET_A(p) ((p)&=~TTL7425_A)
#define TTL7425_PUT_A(p,t) ((p)&=~TTL7425_A,(p)|=((t)?TTL7425_A:0))
#define TTL7425_SET_B(p) ((p)|=TTL7425_B)
#define TTL7425_RESET_B(p) ((p)&=~TTL7425_B)
#define TTL7425_PUT_B(p,t) ((p)&=~TTL7425_B,(p)|=((t)?TTL7425_B:0))
#define TTL7425_SET_C(p) ((p)|=TTL7425_C)
#define TTL7425_RESET_C(p) ((p)&=~TTL7425_C)
#define TTL7425_PUT_C(p,t) ((p)&=~TTL7425_C,(p)|=((t)?TTL7425_C:0))
#define TTL7425_SET_D(p) ((p)|=TTL7425_D)
#define TTL7425_RESET_D(p) ((p)&=~TTL7425_D)
#define TTL7425_PUT_D(p,t) ((p)&=~TTL7425_D,(p)|=((t)?TTL7425_D:0))

#define TTL7425_GET_Y(p) ((p)&TTL7425_Y)

TTL7425_PINS TTL7425[] = {
    0b00010000,
    0b00000001,
    0b00000010,
    0b00000011,
    0b00000100,
    0b00000101,
    0b00000110,
    0b00000111,
    0b00001000,
    0b00001001,
    0b00001010,
    0b00001011,
    0b00001100,
    0b00001101,
    0b00001110,
    0b00001111,
};
