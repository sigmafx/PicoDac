/* 2-INPUT XOR */

typedef unsigned char TTL7486_PINS;

#define TTL7486_A   (1 << 0)
#define TTL7486_B   (1 << 1)
#define TTL7486_Y   (1 << 2)
#define TTL7486_IN  (TTL7486_A|TTL7486_B)

#define TTL7486_INIT   TTL7486_Y
#define TTL7486_ACT(p) (TTL7486[(p)&TTL7486_IN])

#define TTL7486_SET_A(p) ((p)|=TTL7486_A)
#define TTL7486_RESET_A(p) ((p)&=~TTL7486_A)
#define TTL7486_PUT_A(p,t) ((p)&=~TTL7486_A,(p)|=((t)?TTL7486_A:0))
#define TTL7486_SET_B(p) ((p)|=TTL7486_B)
#define TTL7486_RESET_B(p) ((p)&=~TTL7486_B)
#define TTL7486_PUT_B(p,t) ((p)&=~TTL7486_B,(p)|=((t)?TTL7486_B:0))

#define TTL7486_GET_Y(p) ((p)&TTL7486_Y)

TTL7486_PINS TTL7486[] = {
    0b00000100,
    0b00000001,
    0b00000010,
    0b00000111,
};
