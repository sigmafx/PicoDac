/* 2-INPUT NAND */

typedef unsigned char TTL7400_PINS;

#define TTL7400_A   (1 << 0)
#define TTL7400_B   (1 << 1)
#define TTL7400_Y   (1 << 2)
#define TTL7400_IN  (TTL7400_A|TTL7400_B)

#define TTL7400_INIT   TTL7400_Y
#define TTL7400_ACT(p) (TTL7400[(p)&TTL7400_IN])

#define TTL7400_SET_A(p) ((p)|=TTL7400_A)
#define TTL7400_RESET_A(p) ((p)&=~TTL7400_A)
#define TTL7400_PUT_A(p,t) ((p)&=~TTL7400_A,(p)|=((t)?TTL7400_A:0))
#define TTL7400_SET_B(p) ((p)|=TTL7400_B)
#define TTL7400_RESET_B(p) ((p)&=~TTL7400_B)
#define TTL7400_PUT_B(p,t) ((p)&=~TTL7400_B,(p)|=((t)?TTL7400_B:0))

#define TTL7400_GET_Y(p) ((p)&TTL7400_Y)

TTL7400_PINS TTL7400[] = {
    //-----YBA
    0b00000100,
    0b00000101,
    0b00000110,
    0b00000011,
};
