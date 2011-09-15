/*
###################################################################################################################
    This class perform the read of instances of vrp and your variations.
    @author Marcelo
    @date   27/07/2011
    @lastModification -
###################################################################################################################
*/
#ifndef INPUTCANONICALVRP_H
#define INPUTCANONICALVRP_H
#include <QString>

class SOD;

class InputCanonicalVRP
{

public:

    InputCanonicalVRP();
    ~InputCanonicalVRP();
    SOD read(QString inst);
    SOD read();

};

#endif // INPUTCANONICALVRP_H
