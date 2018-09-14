#include <fstream>
#include <cmath>
using namespace std;

ofstream temp_file ("tmp.ps"); // временный файл для сохранения картинки

#include "turtle.h"

Turtle::Turtle(){
  /* Конструктор */
  temp_file << "1 setlinecap" << endl;
  temp_file << "1 setlinejoin" << endl;
}

// --------------- Геттеры и сеттеры ---------------
float Turtle::getCoord(short coord) {
  if (coord == 0)
    return x;
  else
    return y;
}

void Turtle::setCoords(float nx, float ny) {
  x = nx;
  y = ny;
}

float Turtle::getDirection() {
  return direction;
}

void Turtle::setDirection(float ndirection) {
  direction = ndirection;
}

int Turtle::getWidth() {
  return width;
}

void Turtle::setWidth(int wd){
  /* Изменяем ширину линии */
  width = wd;
  temp_file << wd << " setlinewidth" << endl;
}

bool Turtle::getPen() {
  return pen;
}

void Turtle::setPen(bool npen) {
  pen = npen;
}

// --------------- Методы передвижения ---------------

void Turtle::moveto(float x1, float y1){
  /* Переместить черепаху в точку с координатами (x,y) */
  setCoords(x1,y1);
}

void Turtle::move(float distance){
  /* Проползти на distance шагов вперед */
  // расчитываем новые координаты черепахи
  float x1 = getCoord(0) + distance * cos(getDirection() * M_PI / 180);
  float y1 = getCoord(1) + distance * sin(getDirection() * M_PI / 180);

  setPictureSize();

  if (getPen())
    drawLine(x1, y1);

  // устанавливаем новые координаты черепахи
  setCoords(x1, y1);
  setPictureSize();
}

void Turtle::turnRight(float angle){
  /* Поворот черепахи в правую сторону на угол angle */
    setDirection(getDirection()-angle);
}

void Turtle::turnLeft(float angle){
  /* Поворот черепахи в левую сторону на угол angle */
    setDirection(getDirection()+angle);
}

void Turtle::save(){
  save_stack.push(getCoord(0));
  save_stack.push(getCoord(1));
  save_stack.push(getDirection());
}

void Turtle::restore(){
  setDirection(save_stack.top());
  save_stack.pop();
  float nx = save_stack.top();
  save_stack.pop();
  float ny = save_stack.top();
  save_stack.pop();
  setCoords(nx, ny);
}

void Turtle::reset(){
  /* Возвращяет черепаху в исходное положение */
  setCoords(0.0, 0.0);
  setDirection(90.0);
}

// --------------- Методы рисования ---------------

void Turtle::penDown(){
  /* Опускает перо у черепашки, после чего
   * она оставляет след при движении
   */
   setPen(true);
}

void Turtle::penUp(){
  /* Поднимает перо у черепашки, после чего
   * она перестает оставлять след при движении
   */
   setPen(false);
}

void Turtle::drawCircle(float rad){
  /*  Рисует круг радиуса rad с центром
   *  в месте, в котором стоит черепаха.
   */
  if (getPen())
    temp_file << "newpath " << getCoord(0) << " " << getCoord(1) << " " << rad << " 0 360 arc stroke" << endl;
}

void Turtle::drawCircle(float rad, float angle){
  /*  Рисует дугу радиуса rad с углом angle */
  if (getPen()){
    if (angle > 0 && angle <= 360)
      temp_file << "newpath " << getCoord(0) << " " << getCoord(1) << " " << rad << " 0 " << angle << " arc stroke" << endl;
  }
}

void Turtle::drawCircle(float rad, float start, float end){
  /*  Рисует дугу радиуса rad от start до end */
  if (getPen()){
    if (start >= 0 && start < end && end >= 360)
      temp_file << "newpath " << getCoord(0) << " " << getCoord(1) << " " << rad << " " << start << " " << end << " arc stroke" << endl;
  }
}

void Turtle::drawLine(float x1, float y1){
  /* Рисует путь черепахи, если перо опущенно */
  temp_file << "newpath " << getCoord(0) << " " << getCoord(1) <<" moveto "<< x1 << " " << y1 <<" lineto stroke"<<endl;
}

void Turtle::setColor(double red, double green, double blue){
  /* Смена цвета пера черепахи  */
  if (red <= 1 && green <= 1 && blue <= 1){
    temp_file << red << " " << green << " " << blue << " setrgbcolor"<< endl;
  }
}

void Turtle::setColor(int red, int green, int blue){
  /* Смена цвета пера черепахи */
  if (red <= 255 && green <= 255 && blue <= 255) {
    temp_file << (float)red / 255 << " " << (float)green / 255 << " " << (float)blue / 255 << " setrgbcolor"<< endl;
  }
}


void Turtle::setPictureSize(){
  /* Изменение размера итоговой картинки */
  picture_size[0][0] = min(picture_size[0][0], getCoord(0)-getWidth()/2);
  picture_size[0][1] = min(picture_size[0][1], getCoord(1)-getWidth()/2);
  picture_size[1][0] = max(picture_size[1][0], getCoord(0)+getWidth()/2);
  picture_size[1][1] = max(picture_size[1][1], getCoord(1)+getWidth()/2);
}

Turtle::~Turtle(){
  /* Деконструктор */
  temp_file << "showpage" <<endl;
  temp_file.close();
  ifstream temp_file ("tmp.ps");
  ofstream output_file ("output.ps");
  output_file << "%!PS-Adobe-3.0 EPSF-3.0\n";
  output_file << "%%BoundingBox: "<< round(picture_size[0][0]) << " " << round(picture_size[0][1]) << " " << round(picture_size[1][0]) << " " << round(picture_size[1][1]) << "\n";
  output_file << "%%HiResBoundingBox: "<< picture_size[0][0] << " " << picture_size[0][1] << " " << picture_size[1][0] << " " << picture_size[1][1] << "\n";
  output_file << "%%EndComments" <<endl;
  output_file << temp_file.rdbuf();
  // закрываем файлы
  temp_file.close();
  output_file.close();
  remove("tmp.ps"); // удаляем временный файл
}
