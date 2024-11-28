#ifndef CUSTOMDATATYPE_H
#define CUSTOMDATATYPE_H
enum  E_Platform {
    FVC3_1,
    FVC3,
    FVC2_8,
    FVC2_1,
    FVC2
};
enum  E_LanePosition {
    SecClsLe,
    ClsLe,
    ClsRi,
    SecClsRi
};
enum  E_FusionSrc {
    FrontVision_FrontRadar,
    FrontVision_CornerRadar,
    FrontVision,
    FrontRadar,
    CornerRadar,
    None
};
enum E_LaneType{
    CrossEnable,
    CorssDisable
};

enum SwCalibrationAccess{
    ReadOnly,
    ReadWrite
};
#endif // CUSTOMDATATYPE_H

