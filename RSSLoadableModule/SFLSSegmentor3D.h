#ifndef SFLSSegmentor3D_h_
#define SFLSSegmentor3D_h_

#include "SFLS.h"

#include <vector>

// vtk
#include "vtkImageData.h"

class CSFLSSegmentor3D : public CSFLS
{
public:
    typedef short TPixel;
    typedef CSFLSSegmentor3D Self;

    typedef CSFLS SuperClassType;

    typedef SuperClassType::NodeType NodeType;
    typedef SuperClassType::CSFLSLayer CSFLSLayer;

    typedef short ImagePixelType;
    typedef short LabelImagePixelType;
    typedef unsigned char MaskImagePixelType;
    typedef float LevelSetImagePixelType;
    typedef unsigned char NeighborLabelImagePixelType;

    typedef short LabelPixelType;
    typedef unsigned char MaskPixelType;
    typedef float LevelSetPixelType;

    CSFLSSegmentor3D();
    virtual ~CSFLSSegmentor3D() {}


    /* ============================================================
   * functions         */
    void basicInit();

    void setNumIter(unsigned long n);

    void setImage(vtkImageData* img);
    void setMask(vtkImageData* mask);


//    void setSpacing(double dx, double dy, double dz);

    virtual void computeForce() = 0;

    void normalizeForce();

    bool getPhiOfTheNbhdWhoIsClosestToZeroLevelInLayerCloserToZeroLevel(long ix, long iy, long iz, LabelPixelType* mp_label_this_ptr, LevelSetPixelType* mp_phi_this_ptr, LevelSetPixelType& thePhi);

    void oneStepLevelSetEvolution();

    void initializeSFLS() { initializeSFLSFromMask(); }
    void initializeSFLSFromMask(); // m_insideVoxelCount is first computed here

    void initializeLabel();
    void initializePhi();

  //virtual void doSegmenation() = 0;

    // geometry
    double computeKappa(long ix, long iy, long iz);


    void setMaxVolume(double v); // v is in mL
    void setMaxRunningTime(double t); // t in min


    void setCurvatureWeight(double a);

    vtkImageData* getLevelSetFunction();

    /* ============================================================
                       data
    ===============================================================*/
    vtkImageData* mp_img; // current PDF computation assumes this to be an integer valued image, like short
    TPixel* mp_img_ptr; //pointing to the first element of the image

    vtkImageData* mp_label; // short type

    vtkImageData* mp_mask; // unsigned char, 0, non-0 mask for object

    vtkImageData* mp_phi; // float type is enough, no need for double

    vtkImageData* mp_label_mask; // 0-1 mask always corresponding to the 0 and negative region of mp_label

    std::vector< LevelSetPixelType > m_force;

    double m_timeStep;

    unsigned long m_numIter;

    bool m_done;

    unsigned long m_currentIteration;

protected:
    double m_curvatureWeight;

    long m_nx;
    long m_ny;
    long m_nz;

    vtkIdType m_increment0;
    vtkIdType m_increment1;
    vtkIdType m_increment2;


    double m_dx; // in mm
    double m_dy; // in mm
    double m_dz; // in mm

    long m_insideVoxelCount;
    double m_insideVolume;

    double m_maxVolume; // max physical volume, in mm^3
    double m_maxRunningTime; // in sec

    double m_eps;
    double m_pi;



    /*----------------------------------------------------------------------
    These two record the pts which change status

    Because they are created and visited sequentially, and when not
    needed, are clear-ed as a whole. No random insertion or removal is
    needed. So use vector is faster than list.  */
    CSFLSLayer m_lIn2out;
    CSFLSLayer m_lOut2in;


    void updateInsideVoxelCount();


    template<typename FloatType>
    inline bool floatingEqual(FloatType a, FloatType b, FloatType eps = 1e-6)
    {
        return (a-b < eps && b-a < eps);
    }

    bool m_keepZeroLayerHistory;
    std::vector< CSFLSLayer > m_zeroLayerHistory;
};


#endif
