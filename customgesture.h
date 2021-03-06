#ifndef CUSTOMGESTURE_H
#define CUSTOMGESTURE_H
#include <QGesture>
#include <QGestureRecognizer>
#include <QSwipeGesture>

/**

 * @brief The SwipeGestureRecognizer class

 * needed because the defalt implementation of swipe is a little bit odd: it only triggers on 3 finger swipes! (5.3)

 *

 * http://developer.android.com/design/patterns/gestures.html

 * http://qt-project.org/doc/qt-5/gestures-overview.html

 */

class SwipeGestureRecognizer : public QGestureRecognizer

{

public:

    SwipeGestureRecognizer();





private:

    #ifdef FOR_DESKTOP
   static const int MINIMUM_DISTANCE = 50;
#endif
#ifdef FOR_MOBILE
   static const int MINIMUM_DISTANCE = 50;
#endif


   typedef QGestureRecognizer parent;



   bool IsValidMove(int dx, int dy);



   qreal ComputeAngle(int dx, int dy);



   virtual QGesture* create(QObject* pTarget);



   virtual QGestureRecognizer::Result recognize(QGesture* pGesture, QObject *pWatched, QEvent *pEvent);



   void reset (QGesture *pGesture);

};





class SwipeGestureUtil {

public:

    static QSwipeGesture::SwipeDirection GetHorizontalDirection(QSwipeGesture *pSwipeGesture);

    static QSwipeGesture::SwipeDirection GetVerticalDirection(QSwipeGesture *pSwipeGesture);

};

#endif // CUSTOMGESTURE_H
