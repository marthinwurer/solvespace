//-----------------------------------------------------------------------------
// Data structures used frequently in the program, various kinds of vectors
// (of real numbers, not symbolic algebra stuff) and our templated lists.
//
// Copyright 2008-2013 Jonathan Westhues.
//-----------------------------------------------------------------------------
#ifndef SOLVESPACE_DSC_H
#define SOLVESPACE_DSC_H

#include "ss_util.h"

#define CO(v) (v).x, (v).y, (v).z

class Vector;
class Vector4;
class Point2d;
class hEntity;
class hParam;

class Quaternion {
public:
    // a + (vx)*i + (vy)*j + (vz)*k
    double w, vx, vy, vz;

    static const Quaternion IDENTITY;

    static Quaternion From(double w, double vx, double vy, double vz);
    static Quaternion From(hParam w, hParam vx, hParam vy, hParam vz);
    static Quaternion From(Vector u, Vector v);
    static Quaternion From(Vector axis, double dtheta);

    Quaternion Plus(Quaternion b) const;
    Quaternion Minus(Quaternion b) const;
    Quaternion ScaledBy(double s) const;
    double Magnitude() const;
    Quaternion WithMagnitude(double s) const;

    // Call a rotation matrix [ u' v' n' ]'; this returns the first and
    // second rows, where that matrix is generated by this quaternion
    Vector RotationU() const;
    Vector RotationV() const;
    Vector RotationN() const;
    Vector Rotate(Vector p) const;

    Quaternion ToThe(double p) const;
    Quaternion Inverse() const;
    Quaternion Times(Quaternion b) const;
    Quaternion Mirror() const;
};

class Vector {
public:
    double x, y, z;

    static Vector From(double x, double y, double z);
    static Vector From(hParam x, hParam y, hParam z);
    static Vector AtIntersectionOfPlanes(Vector n1, double d1,
                                         Vector n2, double d2);
    static Vector AtIntersectionOfLines(Vector a0, Vector a1,
                                        Vector b0, Vector b1,
                                        bool *skew,
                                        double *pa=NULL, double *pb=NULL);
    static Vector AtIntersectionOfPlaneAndLine(Vector n, double d,
                                               Vector p0, Vector p1,
                                               bool *parallel);
    static Vector AtIntersectionOfPlanes(Vector na, double da,
                                         Vector nb, double db,
                                         Vector nc, double dc, bool *parallel);
    static void ClosestPointBetweenLines(Vector pa, Vector da,
                                         Vector pb, Vector db,
                                         double *ta, double *tb);

    double Element(int i) const;
    bool Equals(Vector v, double tol=LENGTH_EPS) const;
    bool EqualsExactly(Vector v) const;
    Vector Plus(Vector b) const;
    Vector Minus(Vector b) const;
    Vector Negated() const;
    Vector Cross(Vector b) const;
    double DirectionCosineWith(Vector b) const;
    double Dot(Vector b) const;
    Vector Normal(int which) const;
    Vector RotatedAbout(Vector orig, Vector axis, double theta) const;
    Vector RotatedAbout(Vector axis, double theta) const;
    Vector DotInToCsys(Vector u, Vector v, Vector n) const;
    Vector ScaleOutOfCsys(Vector u, Vector v, Vector n) const;
    double DistanceToLine(Vector p0, Vector dp) const;
    double DistanceToPlane(Vector normal, Vector origin) const;
    bool OnLineSegment(Vector a, Vector b, double tol=LENGTH_EPS) const;
    Vector ClosestPointOnLine(Vector p0, Vector deltal) const;
    double Magnitude() const;
    double MagSquared() const;
    Vector WithMagnitude(double s) const;
    Vector ScaledBy(double s) const;
    Vector ProjectInto(hEntity wrkpl) const;
    Vector ProjectVectorInto(hEntity wrkpl) const;
    double DivPivoting(Vector delta) const;
    Vector ClosestOrtho() const;
    void MakeMaxMin(Vector *maxv, Vector *minv) const;
    Vector ClampWithin(double minv, double maxv) const;
    static bool BoundingBoxesDisjoint(Vector amax, Vector amin,
                                      Vector bmax, Vector bmin);
    static bool BoundingBoxIntersectsLine(Vector amax, Vector amin,
                                          Vector p0, Vector p1, bool asSegment);
    bool OutsideAndNotOn(Vector maxv, Vector minv) const;
    Vector InPerspective(Vector u, Vector v, Vector n,
                         Vector origin, double cameraTan) const;
    Point2d Project2d(Vector u, Vector v) const;
    Point2d ProjectXy() const;
    Vector4 Project4d() const;
};

struct VectorHash {
    size_t operator()(const Vector &v) const;
};

struct VectorPred {
    bool operator()(Vector a, Vector b) const;
};

class Vector4 {
public:
    double w, x, y, z;

    static Vector4 From(double w, double x, double y, double z);
    static Vector4 From(double w, Vector v3);
    static Vector4 Blend(Vector4 a, Vector4 b, double t);

    Vector4 Plus(Vector4 b) const;
    Vector4 Minus(Vector4 b) const;
    Vector4 ScaledBy(double s) const;
    Vector PerspectiveProject() const;
};

class Point2d {
public:
    double x, y;

    static Point2d From(double x, double y);
    static Point2d FromPolar(double r, double a);

    Point2d Plus(const Point2d &b) const;
    Point2d Minus(const Point2d &b) const;
    Point2d ScaledBy(double s) const;
    double DivPivoting(Point2d delta) const;
    double Dot(Point2d p) const;
    double DistanceTo(const Point2d &p) const;
    double DistanceToLine(const Point2d &p0, const Point2d &dp, bool asSegment) const;
    double DistanceToLineSigned(const Point2d &p0, const Point2d &dp, bool asSegment) const;
    double Angle() const;
    double AngleTo(const Point2d &p) const;
    double Magnitude() const;
    double MagSquared() const;
    Point2d WithMagnitude(double v) const;
    Point2d Normal() const;
    bool Equals(Point2d v, double tol=LENGTH_EPS) const;
};


class BandedMatrix {
public:
    enum {
        MAX_UNKNOWNS   = 16,
        RIGHT_OF_DIAG  = 1,
        LEFT_OF_DIAG   = 2
    };

    double A[MAX_UNKNOWNS][MAX_UNKNOWNS];
    double B[MAX_UNKNOWNS];
    double X[MAX_UNKNOWNS];
    int n;

    void Solve();
};

#define RGBi(r, g, b) RgbaColor::From((r), (g), (b))
#define RGBf(r, g, b) RgbaColor::FromFloat((float)(r), (float)(g), (float)(b))

// Note: sizeof(class RgbaColor) should be exactly 4
//
class RgbaColor {
public:
    uint8_t red, green, blue, alpha;

    float redF()   const { return (float)red   / 255.0f; }
    float greenF() const { return (float)green / 255.0f; }
    float blueF()  const { return (float)blue  / 255.0f; }
    float alphaF() const { return (float)alpha / 255.0f; }

    bool IsEmpty() const { return alpha == 0; }

    bool Equals(RgbaColor c) const {
        return
            c.red   == red   &&
            c.green == green &&
            c.blue  == blue  &&
            c.alpha == alpha;
    }

    RgbaColor WithAlpha(uint8_t newAlpha) const {
        RgbaColor color = *this;
        color.alpha = newAlpha;
        return color;
    }

    uint32_t ToPackedIntBGRA() const {
        return
            blue |
            (uint32_t)(green << 8) |
            (uint32_t)(red << 16) |
            (uint32_t)((255 - alpha) << 24);
    }

    uint32_t ToPackedInt() const {
        return
            red |
            (uint32_t)(green << 8) |
            (uint32_t)(blue << 16) |
            (uint32_t)((255 - alpha) << 24);
    }

    uint32_t ToARGB32() const {
        return
            blue |
            (uint32_t)(green << 8) |
            (uint32_t)(red << 16) |
            (uint32_t)(alpha << 24);
    }

    static RgbaColor From(int r, int g, int b, int a = 255) {
        RgbaColor c;
        c.red   = (uint8_t)r;
        c.green = (uint8_t)g;
        c.blue  = (uint8_t)b;
        c.alpha = (uint8_t)a;
        return c;
    }

    static RgbaColor FromFloat(float r, float g, float b, float a = 1.0) {
        return From(
            (int)(255.1f * r),
            (int)(255.1f * g),
            (int)(255.1f * b),
            (int)(255.1f * a));
    }

    static RgbaColor FromPackedInt(uint32_t rgba) {
        return From(
            (int)((rgba)       & 0xff),
            (int)((rgba >> 8)  & 0xff),
            (int)((rgba >> 16) & 0xff),
            (int)(255 - ((rgba >> 24) & 0xff)));
    }

    static RgbaColor FromPackedIntBGRA(uint32_t bgra) {
        return From(
            (int)((bgra >> 16) & 0xff),
            (int)((bgra >> 8)  & 0xff),
            (int)((bgra)       & 0xff),
            (int)(255 - ((bgra >> 24) & 0xff)));
    }
};

struct RgbaColorCompare {
    bool operator()(RgbaColor a, RgbaColor b) const {
        return a.ToARGB32() < b.ToARGB32();
    }
};

class BBox {
public:
    Vector minp;
    Vector maxp;

    static BBox From(const Vector &p0, const Vector &p1);

    Vector GetOrigin() const;
    Vector GetExtents() const;

    void Include(const Vector &v, double r = 0.0);
    bool Overlaps(const BBox &b1) const;
    bool Contains(const Point2d &p, double r = 0.0) const;
};

#endif
