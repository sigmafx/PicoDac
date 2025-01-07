/* 3-INPUT NAND */

typedef unsigned char TTL7410_PINS;

#define TTL7410_A   (1 << 0)
#define TTL7410_B   (1 << 1)
#define TTL7410_C   (1 << 2)
#define TTL7410_Y   (1 << 3)
#define TTL7410_IN  (TTL7410_A|TTL7410_B|TTL7410_C)

#define TTL7410_INIT   TTL7410_Y
#define TTL7410_ACT(p) (TTL7410[(p)&TTL7410_IN])

#define TTL7410_SET_A(p) ((p)|=TTL7410_A)
#define TTL7410_RESET_A(p) ((p)&=~TTL7410_A)
#define TTL7410_PUT_A(p,t) ((p)&=~TTL7410_A,(p)|=((t)?TTL7410_A:0))
#define TTL7410_SET_B(p) ((p)|=TTL7410_B)
#define TTL7410_RESET_B(p) ((p)&=~TTL7410_B)
#define TTL7410_PUT_B(p,t) ((p)&=~TTL7410_B,(p)|=((t)?TTL7410_B:0))
#define TTL7410_SET_C(p) ((p)|=TTL7410_C)
#define TTL7410_RESET_C(p) ((p)&=~TTL7410_C)
#define TTL7410_PUT_C(p,t) ((p)&=~TTL7410_C,(p)|=((t)?TTL7410_C:0))

#define TTL7410_GET_Y(p) ((p)&TTL7410_Y)

TTL7410_PINS TTL7410[] = {
    0b00001000,
    0b00001001,
    0b00001010,
    0b00001011,
    0b00001100,
    0b00001101,
    0b00001110,
    0b00000111,
};
