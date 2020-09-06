int irealMouseX() {
  return round((((mouseX-width/2)/scale)-translate.x+width/2));
}
int irealMouseY() {
  return round((((mouseY-height/2)/scale)-translate.y+height/2));
}
