#include "cmbtopo_i.h"
#include "Cmb.h"
#include "Cmb3D.h"

int main(int argc, char** argv){
    cout << "Welcome to cmbtopo" << endl;

    Cmb cmb1;
    
    cmb1.random().seed(141592);

    cmb1.verbose(8);
    cmb1.LSS(1.56); // 1.56 normal
    cmb1.cmbdata().resolution(100000); // 100000 normal
    
    cmb1.maxl(20); // 10 fast, 20 normal
    cmb1.maxn(0); // estimated automatically if set to 0

    cmb1.uberL(10);
    cmb1.maxKLSS(100);
    //cmb1.maxKLSS(50);

    int topology = Cmb::TORUS;
    if (argc > 1) {
        topology = atof(argv[1]);
    }

    switch(topology) {
        case 2:
            cmb1.name("halfturn");
            cmb1.topology(Cmb::HALFTURN);
            cmb1.L(1.0,1.0,2.0);
            break;
        case 3:
            cmb1.name("quarterturn");
            cmb1.topology(Cmb::QUARTERTURN);
            cmb1.L(1.0,1.0,4.0);
            break;
        case 4:
            cmb1.name("thirdturn");
            cmb1.topology(Cmb::THIRDTURN);
            cmb1.L(1.0,1.0,3.0);
            break;
        case 5:
            cmb1.name("chimney");
            cmb1.topology(Cmb::TORUS);
            cmb1.L(1.0,1.0,10.0);
            break;
        case 6:
            cmb1.name("slab");
            cmb1.topology(Cmb::TORUS);
            cmb1.L(1.0,10.0,10.0);
            break;
        case 7:
            cmb1.name("sixthturn");
            cmb1.topology(Cmb::SIXTHTURN);
            cmb1.L(1.0,1.0,6.0);
            break;
        case 8:
            cmb1.name("hantzschewendt");
            cmb1.topology(Cmb::HANTZSCHEWENDT );
            cmb1.L(2.0,2.0,2.0);
            break;
        case 1:
        default:
            cmb1.name("torus");
            cmb1.topology(Cmb::TORUS);
            cmb1.L(1.0, 1.0, 1.0); // 1.0, 1.0, 1.0 normal
            break;
    }

    cmb1.calculateMaxN();
    cmb1.showparams();

    cmb1.build();

    cout << endl << "printalm" << endl;
    cmb1.alm().printalm();

    cout << endl << "printpower" << endl;
    cmb1.alm().printPowers();

    // cout << endl << "printcmb" << endl;
    // cmb1.cmddata().print(&cout);

    double rel1 = cmb1.alm().realcheck();
    double rel2 = cmb1.cmbdata().reality();
    cout << endl;
    cout << "Reality alm: " << rel1 << endl;
    cout << "Reality cmb: " << rel2 << endl;
    cout << endl;

    cout << "Going to draw in 3D hopefully"<<endl;
    Cmb3D cmb3d1;
    cmb3d1.setcmb(&cmb1);
    cmb3d1.start();

    cout << "The End." << endl;
    return 0;    
}
