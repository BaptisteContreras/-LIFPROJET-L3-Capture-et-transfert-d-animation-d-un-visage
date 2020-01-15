//
// Created by baptiste on 12/03/19.
//

#include "worker.h"
#include "math.h"

using namespace std;
using namespace cv;



//*************************** CURRENT WORK ************************************************************

Mat worker::applyFacialExpression2(Mat src, Mat dest, vector<Point2f> land1, vector<Point2f> land2,
                                   vector<Point2f> landMemorized, vector<Vec6f> triangle1, vector<Vec6f> triangle2,
                                   vector<Vec6f> triangleMemorized) {
   // vector<vector<double>> transitionVectors = computeTransitionVector(landMemorized, land1);
    vector<vector<double>> destMouth = computeDestForMouht(landMemorized, land1); // vector for each point



    vector<Point2f> pointsSrcTri = vector<Point2f>();
    vector<Point2f> pointsDestTri = vector<Point2f>();

   // Point2f pt(transitionVectors[0][0],transitionVectors[0][1]);
    //Point2f pt2(transitionVectors[1][0],transitionVectors[1][1]);


    for (int i = 0; i < destMouth.size(); ++i) {
       // Point2f ptDest((land2[i].x+pt.x),(land2[i].y+pt.y));
       // struct pixel r = getPixelRounded(dest,land2[48+i].x,land2[48+i].y);
        //setPixel(dest,ptDest.x,ptDest.y,rounded);

        Point2f ptDest((land2[48+i].x+destMouth[i][0]),(land2[48+i].y+destMouth[i][1])); // final coord for the point
        //  transformImage(dest,destMouth[i],land2[48+i]); // don't work
        cv::arrowedLine(dest,land2[48+i],ptDest ,Scalar(0,255,0));

    }





    return dest;
}

void worker::transformImage(Mat img, vector<double> vect, Point2f base) {
    int rows = img.rows;
    int cols = img.cols;
    int depth = img.depth();


    int pasX = 30;
    int pasY = 30;
    int rowStart = base.x - pasX;
    int rowEnd = base.x + pasX;
    int colStart = base.y - pasY;
    int colEnd = base.y + pasY;
    float poidTotal = 0;
    // calcul poid total


        for (int i = rowStart; i < rowEnd; ++i) {
            for (int j = colStart; j < colEnd; ++j) {
                float poid = 1 / (pixelDistance(j, i, base) + 1);

                poid = 1;
                struct pixel rounded = getPixelRounded(img, base.x, base.y);


               // setPixel(img, (base.y + (vect[1] * poid)), (base.x + (vect[0] * poid)), rounded);
                //setPixel(img,(base.y + (vect[1])),(base.x + (vect[0])),rounded);
                  setPixel(img,(j + (vect[1] * poid)),(i + (vect[0] * poid)),rounded);


            }
        }
    }

// debug
Mat worker::stretchDebug(Mat img, Point2f src, vector<vector<double>> deplacements) {
    Mat small = Mat::zeros(16,16,img.type());

    // cout << "type : " << img.type() << endl;
    struct pixel tochange;
    tochange.R=65;
    tochange.G=255;
    tochange.B=130;
    for (int i = 0; i < 16; ++i) {
        for (int j = 0; j < 16; ++j) {
            tochange.B += i*16;
            tochange.B %= 255;
            tochange.G += i*16;
            tochange.G %= 255;
            tochange.R += i*16;
            tochange.R %= 255;
            setPixel(small,i,j,tochange);
        }
    }
    //setPixel(small,0,1,tochange);$
    cout << "depth : " << img.depth() << " channels : " << img.channels() << endl;
    //cout << 1*img.cols + 1*img.channels() << " yyyyyyy" << endl;
    setPixel(small,1,2,tochange);
    //struct pixel pix = getPixelRounded(small,0.5,0);

    struct pixel pix = getPixelRounded(small,0,0);
    struct pixel pix2 = getPixel(small,0,0);
    cout << "current pixel : " << pix.B  << " " << pix.G << " " << pix.R << endl;
    cout << "current pixel not rounded : " << pix2.B  << " " << pix2.G << " " << pix2.R << endl;


    // debug small
    cout << "small image : " << small << endl;
    return Mat();
}

Mat worker::stretch(Mat img,Point2f src, vector<vector<double>> deplacements) {
    /*
    vector<double> shift_vector = deplacements[0];
    cout << "vector ; " << shift_vector[0] << " " << shift_vector[1] << endl;
    Mat imgCopy = Mat::zeros(img.size(),img.type());
    uchar *destData = img.data;
    uchar *copyData = imgCopy.data;
    // parcour image pour changer tout les pixels

    for (int i = 0; i < img.rows; ++i) {
        for (int j = 0; j < (img.cols*img.channels()); ++j) {
            Point2f srcP(i,j/3);
            Point2f dst((src.x+shift_vector[0]),src.y+shift_vector[1]);
            double helper = 1.0 / pow(3 * (distance(srcP,dst) / shift_vector.size()), 4) + 1;
            // cout << "helper : " << helper << endl;
            double offset[2]={0,0};
            offset[0] -= helper * shift_vector[0];
            offset[1] -= helper * shift_vector[1];
            //Point2f coords;
           // coords.x = clamp(i + int(offset[0]),0,img.size[0] - 1);
           int x,y;
           x = clamp(i + int(offset[0]),0,img.size[0] - 1);
           y = clamp(j + int(offset[1]),0,img.size[1] - 1);

            //coords.y =  clamp(j + int(offset[1]),0,img.size[1] - 1);
            copyData[x*imgCopy.depth()+y]= destData[i*img.depth()+j];


        }

    }
     return imgCopy;

     */





}


vector<vector<double>> worker::computeDestForMouht(vector<Point2f> base, vector<Point2f> dest) {
    vector<vector<double>> toreturn;
    int start = 48;
    int end = 68;
    for (int i = start; i <end ; ++i) {
        vector<double> tmp;
        tmp.push_back(dest[i].x - base[i].x);
        tmp.push_back(dest[i].y - base[i].y);

        toreturn.push_back(tmp);
    }

    return toreturn;
}

int worker::pixelDistance(int x, int y, Point2f base) {
    int res = (base.x+base.y) - (x+y);
    if (res < 0)
        res *= -1;
    return res;
}

//*************************** GETTER AND SETTER for pixel **************************************************


struct pixel worker::getPixelRounded(Mat image, float x, float y) {
    uchar *pixelsMat = image.data;
    int nbChannel = image.channels();
    int xSup = x+1;
    int xinf = x;
    int ySup = y+1;
    int yInf = y;
    float whole = floor(x);
    float xDecimal = x - whole;
    whole = floor(y);
    float yDecimal = y - whole;
    struct pixel pix;
   // cout << "debug : " << xSup << " " <<  xinf << " " << ySup << " " << yInf << " " << xDecimal << " " << yDecimal << " " << endl;

    //* blue component
    int EBlue = (xDecimal * getBlueComponent(image,xinf,yInf)) + (1-xDecimal)*getBlueComponent(image,xSup,yInf);
    int FBlue = ( xDecimal * getBlueComponent(image,xinf,ySup) ) +  (1-xDecimal)*getBlueComponent(image,xSup,ySup);
    int PBlue = (yDecimal * EBlue) + (1-yDecimal)*FBlue;
    pix.B = PBlue;
  //  cout << "debug2 : " << EBlue << " " <<  FBlue << " " << PBlue  << endl;

    //* green component
    int EGreen = (xDecimal * getGreenComponent(image,xinf,yInf)) + (1-xDecimal)*getGreenComponent(image,xSup,yInf);
    int FGreen = ( xDecimal * getGreenComponent(image,xinf,ySup) ) +  (1-xDecimal)*getGreenComponent(image,xSup,ySup);
    int PGreen = (yDecimal * EGreen) + (1-yDecimal)*FGreen;
    pix.G = PGreen;
  //  cout << "debug3 : " << EGreen << " " <<  FGreen << " " << PGreen  << endl;

    //* red component
    int ERed = (xDecimal * getRedComponent(image,xinf,yInf)) + (1-xDecimal)*getRedComponent(image,xSup,yInf);
    int FRed = ( xDecimal * getRedComponent(image,xinf,ySup) ) +  (1-xDecimal)*getRedComponent(image,xSup,ySup);
    int PRed = (yDecimal * ERed) + (1-yDecimal)*FRed;
    pix.R = PRed;
  //  cout << "debug4 : " << ERed << " " <<  FRed << " " << PRed  << endl;

    //cout << "current WTF : " << pix.R  << " " << pix.G << " " << pix.B << endl;



    return pix;
}

struct pixel worker::getPixel(Mat image, int x, int y) {
    uchar *pixelsMat = image.data;
    int nbChannel = image.channels();
    // BGR
    struct pixel pix;
    pix.B =getBlueComponent(image,x,y);
    pix.G = getGreenComponent(image,x,y);
    pix.R = getRedComponent(image,x,y);
    return pix;
}

struct pixelFloat worker::getFloatPixel(Mat image, int x, int y) {
    uchar *pixelsMat = image.data;
    int nbChannel = image.channels();
    // BGR
    struct pixelFloat pix;
    pix.B = (float)pixelsMat[(x*image.cols)*nbChannel + (y*nbChannel)] / 255;
    pix.G = (float)pixelsMat[(x*image.cols)*nbChannel + (y*nbChannel) + 1] / 255;
    pix.R = (float)pixelsMat[(x*image.cols)*nbChannel + (y*nbChannel) + 2] / 255;
    return pix;
}

void worker::setPixel(Mat image, int x, int y, struct pixel pix) {
    setRedChannel(image,x,y,pix.R);
    setGreenChannel(image,x,y,pix.G);
    setBlueChannel(image,x,y,pix.B);
}

void worker::setPixelFloat(Mat image, int x, int y, struct pixelFloat pix) {
    setRedChannel(image,x,y,pix.R*255);
    setGreenChannel(image,x,y,pix.G*255);
    setBlueChannel(image,x,y,pix.B*255);
}

void worker::setRedChannel(Mat image, int x, int y, int val) {
    uchar *pixelsMat = image.data;
    int nbChannel = image.channels();
    // BGR
    pixelsMat[(x*image.cols)*nbChannel + (y*nbChannel)+2] = val;

}

void worker::setGreenChannel(Mat image, int x, int y, int val) {
    uchar *pixelsMat = image.data;
    int nbChannel = image.channels();
    // BGR
    pixelsMat[(x*image.cols)*nbChannel + (y*nbChannel)+1] = val;

}

void worker::setBlueChannel(Mat image, int x, int y, int val) {
    uchar *pixelsMat = image.data;
    int nbChannel = image.channels();
    // BGR
    pixelsMat[(x*image.cols)*nbChannel + (y*nbChannel)] = val;


}

int worker::getRedComponent(Mat image, int x, int y) {
    uchar *pixelsMat = image.data;
    int nbChannel = image.channels();
    // BGR
    return (int)pixelsMat[(x*image.cols)*nbChannel + (y*nbChannel)+2];
}

int worker::getBlueComponent(Mat image, int x, int y) {
    uchar *pixelsMat = image.data;
    int nbChannel = image.channels();
    // BGR
    return (int)pixelsMat[(x*image.cols)*nbChannel + (y*nbChannel)];
}

int worker::getGreenComponent(Mat image, int x, int y) {
    uchar *pixelsMat = image.data;
    int nbChannel = image.channels();
    // BGR
    return (int)pixelsMat[(x*image.cols)*nbChannel + (y*nbChannel) +1];
}

//******************************************* tests*******************************************************

void worker::morphTriangle2(Mat &img1, Mat &imgM, vector<Point2f> &t1, vector<Point2f> &t, double alpha) {
    // Find bounding rectangle for each triangle
    Rect r = boundingRect(t);
    Rect r1 = boundingRect(t1);

    // Offset points by left top corner of the respective rectangles
    vector<Point2f> t1Rect, t2Rect, tRect;
    vector<Point> tRectInt;
    for (int i = 0; i < 3; i++) {
        tRect.push_back(Point2f(t[i].x - r.x, t[i].y - r.y));
        tRectInt.push_back(Point(t[i].x - r.x, t[i].y - r.y)); // for fillConvexPoly

        t1Rect.push_back(Point2f(t1[i].x - r1.x, t1[i].y - r1.y));
    }

    // Get mask by filling triangle
    Mat mask = Mat::zeros(r.height, r.width, CV_32FC3);
    fillConvexPoly(mask, tRectInt, Scalar(1.0, 1.0, 1.0), 16, 0);

    // Apply warpImage to small rectangular patches
    Mat img1Rect, img2Rect;
    img1(r1).copyTo(img1Rect);

    Mat warpImage1 = Mat::zeros(r.height, r.width, img1Rect.type());

    applyAffinTransform(warpImage1, img1Rect, t1Rect, tRect);

    // Alpha blend rectangular patches
    Mat imgRect = warpImage1;

    // Copy triangular region of the rectangular patch to the output image
    multiply(imgRect, mask, imgRect);
    multiply(imgM(r), Scalar(1.0, 1.0, 1.0) - mask, imgM(r));
    imgM(r) = imgM(r) + imgRect;
}

void worker::morphTriangle(Mat &img1, Mat &imgM, vector<Point2f> &t1, vector<Point2f> &t, double alpha) {

    Rect bounding1 = boundingRect(t1);
    Rect bounding2 = boundingRect(t);

    vector<Point2f> t1Rect, t2Rect;
    vector<Point> tRectInt;
    for (int i = 0; i < 3; i++) {
        t1Rect.push_back(Point2f(t[i].x - bounding1.x, t[i].y - bounding1.y));
        tRectInt.push_back(Point(t[i].x - bounding1.x, t[i].y - bounding1.y)); // for fillConvexPoly

        t2Rect.push_back(Point2f(t1[i].x - bounding2.x, t1[i].y - bounding2.y));
    }

    // Get mask by filling triangle
    Mat mask = Mat::zeros(bounding1.height, bounding1.width, CV_32FC3);
    fillConvexPoly(mask, tRectInt, Scalar(1.0, 1.0, 1.0), 16, 0);

    Mat img1Rect, img2Rect;
    img1(bounding2).copyTo(img1Rect);

    Mat warpImage1 = Mat::zeros(bounding1.height, bounding1.width, img1Rect.type());

    applyAffinTransform(warpImage1, img1Rect, t1Rect, t2Rect);

    // Alpha blend rectangular patches
    Mat imgRect = warpImage1;
    img1Rect.copyTo(imgM);
    return;
    // Copy triangular region of the rectangular patch to the output image
    multiply(imgRect, mask, imgRect);
    multiply(imgM(bounding1), Scalar(1.0, 1.0, 1.0) - mask, imgM(bounding1));
    imgM(bounding1) = imgM(bounding1) + imgRect;

}



Mat worker::transferMouthTo(Mat src, Mat dest, Rect rectSrc, Rect rectDest, vector<vector<double>> wrap) {
    uchar *destData = dest.data;
    int channels = dest.channels();


    int start1 = rectDest.y;
    int end1 = rectDest.y + rectSrc.height;
    int start2 = rectDest.x;
    int end2 = rectDest.x + rectSrc.width;
    int row = 0;
    int col = 0;
    for (int i = start1; i < end1; ++i) {
        // double *p = dest.ptr<double>(i);
        for (int j = start2; j < end2; ++j) {

            destData[(i * dest.step) + (j * channels)] = wrap[row][col];
            col++;
            destData[(i * dest.step) + (j * channels) + 1] = wrap[row][col];
            col++;
            destData[(i * dest.step) + (j * channels) + 2] = wrap[row][col];
            col++;
        }
        row++;
        col = 0;
    }


    return dest;
}

vector<vector<double>> worker::wrapZone(Mat img, Rect rect) {
    vector<vector<double>> tmp(img.rows, vector<double>(img.cols * img.channels())); // row[col]
    uchar *srcData = img.data;
    int row = 0;
    int col = 0;
    for (int i = rect.y; i < rect.y + rect.height; ++i) {

        for (int j = rect.x; j < (rect.width + rect.x); ++j) {
            tmp[row][col] = (double) srcData[(i * img.step) + (j * 3)];
            col++;
            tmp[row][col] = (double) srcData[(i * img.step) + (j * 3) + 1];
            col++;
            tmp[row][col] = (double) srcData[(i * img.step) + (j * 3) + 2];
            col++;

        }
        row += 1;
        col = 0;
    }
    return tmp;
}

Mat
worker::applyFacialExpression(Mat src, Mat dest, vector<Point2f> land1, vector<Point2f> land2, vector<Vec6f> triangle1,
                              vector<Vec6f> triangle2) {
    vector<Point2f> morphedPoints = worker::determineNewLandmarks(land1, land2);
    double dist, dist2; // sqrt((x2-x1)œ + (y2-y1)œ)
    // dist = sqrt(pow(land2[48].x-land2[59].x,2)+pow(land2[48].y-land2[59].y,2) );
    if (land1.empty() || land2.empty() || triangle1.empty() || triangle2.empty())
        return dest;
    //  dist = distance(land1[48],land1[59]);
    // dist = distance(land1[0],land1[16]);
    //dist2 = distance(land2[0],land2[16]);

    // land 1 landmarks for the first image
    // land 2 Landmarks for the second image
    // morphedPoints Landmarks for the combinaison of the two images
    Mat test = dest.clone();


    vector<Point2f> pointsTri1 = triangle2points(triangle1);
    vector<Point2f> pointsTri2 = triangle2points(triangle2);
    Rect mouthArea1 = boundingRect(pointsTri1);
    Rect mouthArea2 = boundingRect(pointsTri2);
    // Rect mouthArea3 = boundingRect(morphedPoints);
    //  cout << (double)dest.data[100 * dest.step + 9] << " debuuuug " << dest.depth() << " " << dest.channels() << endl;
    //rectangle(dest, mouthArea2, Scalar(0, 255, 0));
    //rectangle(src, mouthArea1, Scalar(0, 255, 0));

    vector<vector<double>> mouthWrapped = wrapZone(src, mouthArea1);

    dest = transferMouthTo(src, dest, mouthArea1, mouthArea2, mouthWrapped);

    return dest;
}

int worker::clamp(int num, int min_val,int max_val) {
    return max(min(num, max_val), min_val);
}


vector<Point2f> worker::determineNewLandmarks(vector<Point2f> img1, vector<Point2f> img2) {
    // just for the mouth
    // 48 -> 59 Outer lip
    // 60 -> 67 Inner lip
    int start = 0;
    int end = 68; // < end
    float x, y;
    float alpha = 0.5; // plus il est grand plus la deuxieme image aura de l'importance
    vector<Point2f> points;
    for (int i = start; i < end; ++i) {
        x = (1 - alpha) * img1[i].x + alpha * img2[i].x;
        y = (1 - alpha) * img1[i].y + alpha * img2[i].y;
        points.push_back(Point2f(x, y));
    }

    return points;
}



double worker::distance(Point2f a, Point2f b) {
    return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2));
}

vector<Point2f> worker::triangle2points(vector<Vec6f> triangle) {
    vector<Point2f> pointsTri = vector<Point2f>();
    for (int i = 0; i < triangle.size(); ++i) {
        Point2f ptsTmp1 = Point2f();
        Point2f ptsTmp2 = Point2f();
        Point2f ptsTmp3 = Point2f();

        ptsTmp1.x = triangle[i][0];
        ptsTmp1.y = triangle[i][1];

        ptsTmp2.x = triangle[i][2];
        ptsTmp2.y = triangle[i][3];

        ptsTmp3.x = triangle[i][4];
        ptsTmp3.y = triangle[i][5];

        pointsTri.push_back(ptsTmp1);
        pointsTri.push_back(ptsTmp2);
        pointsTri.push_back(ptsTmp3);

    }

    return pointsTri;

}



vector<vector<double>> worker::computeTransitionVector(vector<Point2f> base, vector<Point2f> dest) {
    // 48 -> 59 Outer lip
    // 60 -> 67 Inner lip
    vector<vector<double>> container = vector<vector<double>>();
    vector<double> tmp = vector<double>();
    tmp.push_back(dest[48].x - base[48].x);
    tmp.push_back(dest[48].y - base[48].y);
    container.push_back(tmp);
    tmp = vector<double>();
    tmp.push_back(dest[59].x - base[59].x);
    tmp.push_back(dest[59].y - base[59].y);
    container.push_back(tmp);
    // container[0] == vector for top left point
    // container[1] == vector for top right point
    return container;
}






void worker::applyAffinTransform(Mat &warpImage, Mat &src, vector<Point2f> &srcTri, vector<Point2f> &dstTri) {
    // Given a pair of triangles, find the affine transform.
    Mat warpMat = getAffineTransform( srcTri, dstTri );

    // Apply the Affine Transform just found to the src image
    warpAffine( src, warpImage, warpMat, warpImage.size(), INTER_LINEAR, BORDER_REFLECT_101);
}





vector<Vec6f> worker::triangulationMorphed(Mat f, vector<Point2f>  landmarks) {

    // Rectangle to be used with Subdiv2D
    Size size = f.size();
    Rect rect(0, 0, size.width, size.height);

    // Create an instance of Subdiv2D
    Subdiv2D subdiv(rect);

    // Create a vector of points.
    vector<Point2f> points;

    // just for the mouth
    // 48 -> 59 Outer lip
    // 60 -> 67 Inner lip
    int start = 48;
    int end = 68; // < end

    for (int i = start ; i < end;i++){
        if (landmarks[i].x > 640 || landmarks[i].y > 480){
            //cout << "coordonate out of image bounds, unable to draw tiangulation, skip instead of crash" << endl;

        }else{

            points.push_back(Point2f(landmarks[i].x,landmarks[i].y));
        }
        // cout << landmarks[0][i].x;
    }


    // Insert points into subdiv
    for( vector<Point2f>::iterator it = points.begin(); it != points.end(); it++)
    {


        if (it->x <= 640 && it->x >= 0 && it->y <= 480 && it->y >= 0){

            subdiv.insert(*it);
        }


    }
    vector<Vec6f> triangleList;
    subdiv.getTriangleList(triangleList);

    return triangleList;
}

Mat worker::morphTriangle3(Mat &img1, Mat &img2, Mat &img, vector<Point2f> &t1, vector<Point2f> &t2, vector<Point2f> &t,
                           double alpha) {
    // Find bounding rectangle for each triangle
    Rect r = boundingRect(t);
    Rect r1 = boundingRect(t1);
    Rect r2 = boundingRect(t2);

    // Offset points by left top corner of the respective rectangles
    vector<Point2f> t1Rect, t2Rect, tRect;
    vector<Point> tRectInt;
    for(int i = 0; i < 3; i++)
    {
        tRect.push_back( Point2f( t[i].x - r.x, t[i].y -  r.y) );
        tRectInt.push_back( Point(t[i].x - r.x, t[i].y - r.y) ); // for fillConvexPoly

        t1Rect.push_back( Point2f( t1[i].x - r1.x, t1[i].y -  r1.y) );
        t2Rect.push_back( Point2f( t2[i].x - r2.x, t2[i].y - r2.y) );
    }

    // Get mask by filling triangle
    Mat mask = Mat::zeros(r.height, r.width, 16);
    fillConvexPoly(mask, tRectInt, Scalar(1.0, 1.0, 1.0), 16, 0);

    // Apply warpImage to small rectangular patches
    Mat img1Rect, img2Rect;
    img1(r1).copyTo(img1Rect);
    img2(r2).copyTo(img2Rect);

    Mat warpImage1 = Mat::zeros(r.height, r.width, img1Rect.type());
    Mat warpImage2 = Mat::zeros(r.height, r.width, img2Rect.type());

    applyAffinTransform(warpImage1, img1Rect, t1Rect, tRect);
    applyAffinTransform(warpImage2, img2Rect, t2Rect, tRect);

    // Alpha blend rectangular patches
    Mat imgRect = (1.0 - alpha) * warpImage1 + alpha * warpImage2;
    //cout << "type of :" << mask.type() << " img rect: "<<img1Rect.type() << endl;
    // Copy triangular region of the rectangular patch to the output image
    multiply(imgRect,mask, imgRect,1,16);
    multiply(img(r), Scalar(1.0,1.0,1.0) - mask, img(r),1,21);
    // img(r) = img(r) + imgRect;

    return img;

}
















