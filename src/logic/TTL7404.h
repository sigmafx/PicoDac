/* INVERTER */

typedef unsigned char TTL7404_PINS;

#define TTL7404_A   (1 << 0)
#define TTL7404_Y   (1 << 1)
#define TTL7404_IN  (TTL7404_A)

#define TTL7404_INIT   TTL7404_Y
#define TTL7404_ACT(p) (TTL7404[(p)&TTL7404_IN])

#define TTL7404_SET_A(p) ((p)|=TTL7404_A)
#define TTL7404_RESET_A(p) ((p)&=~TTL7404_A)
#define TTL7404_PUT_A(p,t) ((p)&=~TTL7404_A,(p)|=((t)?TTL7404_A:0))

#define TTL7404_GET_Y(p) ((p)&TTL7404_Y)

TTL7404_PINS TTL7404[] = {
    0b00000010,
    0b00000001
};
