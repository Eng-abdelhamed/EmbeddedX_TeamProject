#define SET_BIT(VAR,bitpos)     VAR|=(1<<bitpos)
#define CLR_BIT(VAR,bitpos)     VAR&=~(1<<bitpos)
#define TOGGLE_BIT(VAR,bitpos)  VAR^=(1<<bitpos)
#define GET_BIT(VAR,bitpos)     (VAR>>bitpos)&1






