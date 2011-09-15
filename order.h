/*
###################################################################################################################
    This class represent a order in SOD.
    @author Marcelo
    @date   27/07/2011
###################################################################################################################
*/
#ifndef ORDER_H
#define ORDER_H

class Order
{

private:

    int       iPriority;
    int       iIndex;
    long int  iCoordX;
    long int  iCoordY;
    float     nDemand;

public:

    Order();
    Order(int iIndex, long int iCoordX, long int iCoordY, float nDemand, int iPriority);
    float      getDemand() const;
    long int   getCoordX() const;
    long int   getCoordY() const;
    int        getIndex() const;
    int        getPriority() const;
    void       setDemand(float nDemand);
    void       setCoordX(long int iCoordX);
    void       setCoordY(long int iCoordY);
    void       setPriority(int iPriority);
    void       setIndex(int iIndex);

};

#endif // ORDER_H
