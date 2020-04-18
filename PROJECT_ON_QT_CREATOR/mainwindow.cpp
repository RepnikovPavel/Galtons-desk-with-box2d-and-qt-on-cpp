#include "mainwindow.h"
#include "ui_mainwindow.h"


vector<b2Body*> vec_with_dinamic_balls;
int numbers_of_balls=0;

vector<Circle*> vec_with_dinamic_ellipse;

vector<double> vec_with_x_of_static_balls;

vector<RectForHist* > vec_with_rect_for_hist;
double y_of_hist;
bool flag_for_clean_hist=0;
bool flag_for_hist=0;

double h_for_hist;
double x_min_for_hist;
double x_max_for_hist;
int num_of_bins;



vector<CircleForTr*> vec_with_circle_for_trajectory;


#define SCALE 200
qreal fromB2(qreal value)
{
    return value*SCALE;
}
qreal toB2(qreal value)
{
    return value / SCALE;
}


void make_hist(Scene* scene)
{
    vector<double> v = vec_with_x_of_static_balls;
//    double x_max= *std::max_element( v.begin(), v.end() );
//    double x_min= *std::min_element( v.begin(), v.end() );

//    double R = x_max- x_min; //ширина гистограммы
//    int k= sqrt(vec_with_x_of_static_balls.size())-2.0; // рекомендуемое sqrt(n)+-2
//    double h= R / k; //ширина интревала

    double x_max = x_max_for_hist;
    double x_min = x_min_for_hist;
    double R = x_max- x_min;
    int k= num_of_bins;
    double h= R / k; //ширина интревала

    vector<int> tmp_vec;
    //аналог сортироваки пузырьком
    for(int i=0; i< k;i++)
    {
        tmp_vec.push_back(0);
        for(int j=0;j< vec_with_x_of_static_balls.size();j++)
        {
            if(i != 0 ){

                if(vec_with_x_of_static_balls[j] <= x_min + i* h && vec_with_x_of_static_balls[j] > x_min + (i-1)* h )
                {
                    tmp_vec[i] += 1;
                }

            }

        }

    }

    for(int i =0; i < tmp_vec.size();i++)
    {
        double l = tmp_vec[i]* scene->r;
        double width_of_palka= h;


        RectForHist* tmp_rect= new RectForHist(QSizeF(l,width_of_palka), QPointF(x_min+i*h, y_of_hist + l/2 ), 90);

        scene->addItem(tmp_rect);

        vec_with_rect_for_hist.push_back(tmp_rect);

    }

}
void clean_hist(Scene* scene){
    for (int i =0 ;i< vec_with_rect_for_hist.size();i++) {

       scene->removeItem(vec_with_rect_for_hist[i]);
    }
    vec_with_rect_for_hist.clear();
}
void classic(Scene* scene,b2World* world, double a ,double b, double Ny,double depth ,double R,double r,double gamma,double k){

    double local_height = b / Ny;
    int i_max= Ny;
    double x_step= 2*R+r*8;//шаг между соседними центрами
    int j_max= a /x_step;
    double l= local_height -R-2*r; //длина палочек около каждого шарика
    double pr = depth + l; //длина палочек для ячеек
    double width_of_palka = r;
    if (j_max%2==0){
        scene->zentr =( j_max / 2.0 * x_step) ;
        scene->x_of_generator= scene->zentr;
    }
    if(j_max%2!=0){
         scene->zentr = ((j_max-1)/ 2.0 *x_step);
          scene->x_of_generator= scene->zentr;
    }
    for(int i = 0; i < i_max;i++ ){
        if(i%2==0){
            for(int j =0;j< j_max;j++){
                scene->addItem(new Circle(world, R, QPointF(x_step*j, local_height*i ), false, gamma,k ) );
//                scene->addItem(new GroundRect(world,QSizeF(l,width_of_palka), QPointF(x_step*j +R-width_of_palka/2, local_height*i +l /2.0), 90));
//                scene->addItem(new GroundRect(world,QSizeF(l,width_of_palka), QPointF(x_step*j -R+width_of_palka/2, local_height*i +l /2.0), 90));
                if(i >= i_max-1)
                {
                   scene->addItem(new GroundRect(world,QSizeF(pr,width_of_palka), QPointF(x_step*j +R-width_of_palka/2, b + depth/2 ), 90));
                   scene->addItem(new GroundRect(world,QSizeF(pr,width_of_palka), QPointF(x_step*j -R+width_of_palka/2,  b + depth/2), 90));
                }
            }
        }
        else{
            for(int j =0;j< j_max;j++){
                scene->addItem(new Circle(world, R, QPointF(x_step*j+x_step/2, local_height*i ), false, gamma ,k) );
//                scene->addItem(new GroundRect(world,QSizeF(l,width_of_palka), QPointF(x_step*j +R-width_of_palka/2+x_step/2, local_height*i +l /2.0), 90));
//                scene->addItem(new GroundRect(world,QSizeF(l,width_of_palka), QPointF(x_step*j -R+width_of_palka/2+x_step/2, local_height*i +l /2.0), 90));
                if(i >= i_max-1)
                {
                   scene->addItem(new GroundRect(world,QSizeF(pr,width_of_palka), QPointF(x_step*j +R-width_of_palka/2+x_step/2, b+ depth/2 ), 90));
                   scene->addItem(new GroundRect(world,QSizeF(pr,width_of_palka), QPointF(x_step*j -R+width_of_palka/2+x_step/2,  b+ depth/2), 90));
                }
            }

        }
    }


    scene->addItem(new GroundRect(world,QSizeF(a,r), QPointF(a/2, b+ depth/2 + pr/2 ),0));

    h_for_hist = x_step ;
    x_min_for_hist =-R+width_of_palka/2+x_step/2;
    x_max_for_hist= x_step*(j_max-1) -R+width_of_palka/2+x_step/2;
    num_of_bins = j_max - 1;

}

void ravn(Scene* scene,b2World* world,double a ,double b,double Nx2,double Ny2,double depth ,double R,double r,double gamma,double k ){

    int m = Ny2;
    int n = Nx2;
    vector<vector<double>> X(m, vector<double>(n));
    vector<vector<double>> Y(m, vector<double>(n));

    srand(time(NULL));
    for (int i = 0; i < m; i++){

        for (int j = 0; j < n; j++){
            X[i][j] = Random::get(0, 10000)/ 10000.0 * a;
//            X[i][j] = rand01() * a  ;
        }
    }

    for (int i = 0; i < m; i++){

        for (int j = 0; j < n; j++){

            Y[i][j] = Random::get(0, 10000)/ 10000.0 * b;
        }
    }

    scene->zentr = a/2;
    scene->x_of_generator= a/2;
    for (int i = 0; i < m; i++){

        for (int j = 0; j < n; j++){
            scene->addItem(new Circle(world, R, QPointF(X[i][j], Y[i][j] ), false ,gamma,k) );
        }
    }

    double width_of_palka = r;
    double pr = depth;
    double l_of_floor= 1.5*a;
    scene->addItem(new GroundRect(world,QSizeF(l_of_floor,width_of_palka), QPointF(a/2, b+ depth/2 + pr/2 ),0));

    double x_step= 2*R ;

    int i_max= l_of_floor / x_step;

    for(int i =0 ; i < i_max ;i++ ){
        scene->addItem(new GroundRect(world,QSizeF(pr ,width_of_palka), QPointF(i* x_step - (l_of_floor-a)/2 , b+ depth/2+ R),90));
    }

    h_for_hist = x_step ;
    x_min_for_hist =- (l_of_floor-a)/2;
    x_max_for_hist= (i_max-1)* x_step - (l_of_floor-a)/2;
    num_of_bins = i_max - 1;

}

void make_tr(Scene* scene)
{
    ;
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    ui->setupUi(this);

}

MainWindow::~MainWindow()
{
    delete ui;
}
Scene::Scene(qreal x, qreal y ,qreal width,qreal height,b2World* world)
    :QGraphicsScene(fromB2(x), fromB2(y),fromB2(width), fromB2(height) )
{
    this->world =world;
}
void Scene::advance()
{
    if(flag_for_clean_hist==1)
    {
        clean_hist(this);
    }
    world->Step(1.00f/60.00f,6,2);
    QGraphicsScene::advance();


    // удаление лишних шариков
    vector<b2Body*> tmp_vec;
    vector<Circle*> tmp_vec2;
    for(int i =0 ; i < vec_with_dinamic_balls.size();i++)
    {
        if(vec_with_dinamic_balls[i]->GetPosition().y >= y_for_ball_out ){
            double tmp_x = vec_with_dinamic_balls[i]->GetPosition().x;
            vec_with_x_of_static_balls.push_back(tmp_x);
            vec_with_dinamic_balls[i]->GetWorld()->DestroyBody(vec_with_dinamic_balls[i]);
            this->removeItem(vec_with_dinamic_ellipse[i]);
            continue;
        }
        //Для отрисовки траекторий
        double x_for_static_circle= vec_with_dinamic_balls[i]->GetPosition().x;
        double y_for_static_circle= vec_with_dinamic_balls[i]->GetPosition().y;
        CircleForTr* tmp_static_circle = new CircleForTr(this->r, QPointF(x_for_static_circle,y_for_static_circle));
        vec_with_circle_for_trajectory.push_back(tmp_static_circle);


        tmp_vec.push_back(vec_with_dinamic_balls[i]);
        tmp_vec2.push_back(vec_with_dinamic_ellipse[i]);
    }
    vec_with_dinamic_balls.clear();
    vec_with_dinamic_ellipse.clear();

    for(int i =0 ; i < tmp_vec.size();i++){
        vec_with_dinamic_balls.push_back(tmp_vec[i]);
        vec_with_dinamic_ellipse.push_back(tmp_vec2[i]);

    }

    if(flag_for_hist==1)
    {
        make_hist(this);
        flag_for_clean_hist=1;
    }





}
Circle::Circle(b2World* world, qreal radius, QPointF initPos,bool type_of_body,double gamma,double k)
    : QGraphicsEllipseItem(0)
{
    setRect(-fromB2(radius),-fromB2(radius),fromB2(radius*2),fromB2(radius*2));

    setPos(fromB2(initPos.x()),fromB2(initPos.y()));
    b2BodyDef bodyDef;
    if(type_of_body==true){
        bodyDef.type= b2_dynamicBody;
        bodyDef.linearDamping = gamma; //
        setBrush(QBrush(Qt::green));
    }
    if(type_of_body==false){
        bodyDef.type= b2_staticBody;
        setBrush(QBrush(Qt::red));
    }
    bodyDef.position.Set(initPos.x(),initPos.y());
    body = world->CreateBody(&bodyDef);

    b2CircleShape shape;
    shape.m_radius= radius;

    b2Fixture* fixture = body->CreateFixture(&shape,1.00f);
    fixture->SetRestitution(k); //коэффициент упругости

    if(type_of_body==true){
        vec_with_dinamic_balls.push_back(body);
        vec_with_dinamic_ellipse.push_back(this);
    }


}
Circle:: ~Circle()
{
    body->GetWorld()->DestroyBody(body);

}
void Circle::advance(int phase)
{
    if (phase)
    {
        setPos(fromB2(body->GetPosition().x),
               fromB2(body->GetPosition().y));
    }
}
GroundRect::GroundRect(b2World *world ,QSizeF size,QPointF initPos ,qreal angle)
    :QGraphicsRectItem()
{
    setRect(-fromB2(size.width()/2),-fromB2(size.height()/2),
            fromB2(size.width()),fromB2(size.height()));
    setBrush(QBrush(Qt::black));
    setPos(fromB2(initPos.x()),
           fromB2(initPos.y()));
    setRotation(angle);//принимает в градусах


    b2BodyDef bodyDef;
    bodyDef.type= b2_staticBody;
    bodyDef.position.Set(initPos.x(),initPos.y());
    bodyDef.angle = 3.14* angle / 180 ;//в радианах

    body = world->CreateBody(&bodyDef);

    b2PolygonShape shape;
    shape.SetAsBox(size.width()/2,size.height()/2);

    body->CreateFixture(&shape,0.00f);
}
GroundRect::~GroundRect(){
    body->GetWorld()->DestroyBody(body);

}


void Scene::mousePressEvent(QGraphicsSceneMouseEvent *event){
//    addItem(new Circle(world, this->r ,
//                       QPointF(toB2(event->scenePos().x()),
//                               toB2(event->scenePos().y())), true, this->gamma,this->k )  );

    this->x_of_generator= toB2(event->scenePos().x());
    this->y_of_generator= toB2(event->scenePos().y());
}
void Scene::wheelEvent(QGraphicsSceneWheelEvent *event){
    if (event->delta()>0)
    {
        this->qV->scale(1.1,1.1);
    }
    if (event->delta()<0)
    {
       this->qV->scale(0.9,0.9);
    }

}

void Scene::create_body(){
    srand(time(NULL));

    double x= this->x_of_generator + (rand01()-0.5)*2*R;
    double y= this->y_of_generator;
    Circle* tmp = new Circle(world, r ,QPointF(x,y), true,this->gamma,this->k );
    addItem( tmp );
    numbers_of_balls += 1;
    qDebug()<<numbers_of_balls;
}
void MainWindow::on_pushButton_3_clicked()
{
   frameTimer2->start(1000/scene->B_per_sek);
}

void MainWindow::on_pushButton_2_clicked()
{
    frameTimer2->stop();
}

void MainWindow::on_pushButton_clicked()
{
    QString b_text = ui->lineEdit->text();
    double b= b_text.right( b_text.size() - (b_text.indexOf('=')+1) ).toDouble();

    QString a_text = ui->lineEdit_2->text();
    double a= a_text.right( a_text.size() - (a_text.indexOf('=')+1) ).toDouble();

    QString depth_text = ui->lineEdit_3->text();
    double depth= depth_text.right( depth_text.size() - (depth_text.indexOf('=')+1) ).toDouble();

    QString r_text = ui->lineEdit_4->text();
    double r= r_text.right( r_text.size() - (r_text.indexOf('=')+1) ).toDouble();

    QString R_text = ui->lineEdit_5->text();
    double R= R_text.right( R_text.size() - (R_text.indexOf('=')+1) ).toDouble();

    QString Ny_text = ui->lineEdit_6->text();
    double Ny= Ny_text.right( Ny_text.size() - (Ny_text.indexOf('=')+1) ).toDouble();

    QString g_text = ui->lineEdit_7->text();
    double g= g_text.right( g_text.size() - (g_text.indexOf('=')+1) ).toDouble();

    QString k_text = ui->lineEdit_8->text();
    double k= k_text.right( k_text.size() - (k_text.indexOf('=')+1) ).toDouble();

    QString gamma_text = ui->lineEdit_9->text();
    double gamma= gamma_text.right( gamma_text.size() - (gamma_text.indexOf('=')+1) ).toDouble();

    QString Nx2_text = ui->lineEdit_10->text();
    double Nx2= Nx2_text.right( Nx2_text.size() - (Nx2_text.indexOf('=')+1) ).toDouble();

    QString Ny2_text = ui->lineEdit_11->text();
    double Ny2= Ny2_text.right( Ny2_text.size() - (Ny2_text.indexOf('=')+1) ).toDouble();

    QString B_per_sek_text = ui->lineEdit_12->text();
    double B_per_sek= B_per_sek_text.right( B_per_sek_text.size() - (B_per_sek_text.indexOf('=')+1) ).toDouble();


    world = new b2World(b2Vec2(0.00f,g));

    double tmp_y= b + depth;
    double tmp_x = a;

                        // точка в которой построится гистограмма на сцене
    y_of_hist = b+depth;
    scene = new Scene(0,0,tmp_x ,tmp_y, world);
    ui->graphicsView->setScene(scene);
    scene->qV = ui->graphicsView;
    scene->r= r;
    scene->gamma= gamma;
    scene->k= k;
    scene->R=R;
    scene->B_per_sek=B_per_sek;

    scene->y_of_generator =-2*R;
    scene->y_for_ball_out= b+depth/2;

    if(ui->comboBox->currentText() == "ravn"){
        ravn(scene, world,a,b,Nx2,Ny2,depth,R,r,gamma,k);
    }
    if(ui->comboBox->currentText() == "classic"){
        classic(scene, world,a,b,Ny,depth,R,r,gamma,k);
    }

    frameTimer = new QTimer(this);
    connect(frameTimer,SIGNAL(timeout()),
            scene,SLOT(advance()));

    frameTimer->start(1000/60);


    frameTimer2 = new QTimer(this);
    connect(frameTimer2,SIGNAL(timeout()),
            scene,SLOT(create_body()));
}



void MainWindow::on_pushButton_4_clicked()
{

    for(int i =0 ; i < vec_with_dinamic_balls.size();i++)
    {
        vec_with_dinamic_balls[i]->SetType(b2_staticBody);
    }

    vec_with_dinamic_balls.clear();

}

void MainWindow::on_pushButton_5_clicked()
{
    //make_hist(scene);
    flag_for_hist=1;
}


RectForHist::RectForHist(QSizeF size, QPointF initPos, qreal angle) :QGraphicsRectItem()
{
    setRect(-fromB2(size.width()/2),-fromB2(size.height()/2),
            fromB2(size.width()),fromB2(size.height()));
    setBrush(QBrush(Qt::blue));
    setPos(fromB2(initPos.x()),
           fromB2(initPos.y()));
    setRotation(angle);//принимает в градусах
}


void MainWindow::on_pushButton_6_clicked()
{
    clean_hist(scene);
    flag_for_hist =0;
}

void MainWindow::on_pushButton_7_clicked()
{
    for (int i=0;i< vec_with_circle_for_trajectory.size();i++) {
        this->scene->addItem(vec_with_circle_for_trajectory[i]);
    }
}

CircleForTr::CircleForTr(qreal radius, QPointF initPos)
    : QGraphicsEllipseItem(0)
{
    setRect(-fromB2(radius),-fromB2(radius),fromB2(radius*2),fromB2(radius*2));
    setBrush(QBrush(Qt::green));
    setPos(fromB2(initPos.x()),fromB2(initPos.y()));
}

void MainWindow::on_pushButton_8_clicked()
{
    for (int i=0;i< vec_with_circle_for_trajectory.size();i++) {
        this->scene->removeItem(vec_with_circle_for_trajectory[i]);
    }
}
