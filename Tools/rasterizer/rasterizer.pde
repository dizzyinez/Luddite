import controlP5.*;
ControlP5 gui;

//PShape character;

//int frames = 35;

PImage sprite_sheet_data;
PImage sprite_sheet_visible;

void setup()
{
  size(1000,700, P2D);
  surface.setResizable(true);
  noSmooth();
  hint(DISABLE_TEXTURE_MIPMAPS);
  gui = new ControlP5(this);
  ((PGraphicsOpenGL)g).textureSampling(3);
  //surface.setResizable(true);
  //sprite_sheet = createImage(0,0, ARGB);
  init_render();
  init_gui();
}


PVector translate = new PVector(0, 0);
float scale = 1;
void draw()
{
  background(150);
  selected_pixel_data_display.setText("");
  if ( mousePressed )
  {
    if (mouseButton == RIGHT)
    {
      translate.x += (mouseX - pmouseX) / scale;
      translate.y += (mouseY - pmouseY) / scale;
    }
  }
  pushMatrix();
  translate(width/2, height/2);
  scale(scale);
  translate(-width/2, -height/2);
  translate(translate.x, translate.y);
  if (sprite_sheet_visible != null)
  {
    //image(sprite_sheet, -sprite_sheet.width/2, -sprite_sheet.height/2);
    switch (round(image_selection.getValue()))
    {
      case 0:
        image(sprite_sheet_visible.get(), 0,0);
        break;
      case 1:
        image(sprite_sheet_data.get(), 0,0);
        break;
    }
  }
  popMatrix();
  
  if ( mousePressed )
  {
    if (mouseButton == LEFT)
    {
      if (sprite_sheet_visible != null)
      {
        fill(0xBB444444);
        noStroke();
        rect(0,height-20, width, height);
        color pixel_vis = sprite_sheet_visible.get(irealMouseX(), irealMouseY());
        color pixel_data = sprite_sheet_data.get(irealMouseX(), irealMouseY());
        selected_pixel_data_display.setText(
        "X: "  + nf(irealMouseX(), 4) +
        " Y: " + nf(irealMouseY(), 4) +
        " | " + 
        "Red: "          + nf(round(red(pixel_vis)),   3) + 
        " Green: "       + nf(round(green(pixel_vis)), 3) + 
        " Blue: "        + nf(round(blue(pixel_vis)),  3) + 
        " Alpha: "       + nf(round(alpha(pixel_vis)), 3) +  
        " Color Index: " + round(alpha(pixel_data)));
      }
    }
  }
}

void mouseWheel(MouseEvent event) {
  float e = event.getCount();
  float oldScale = scale;
  scale *= pow(1.1, -e);
  translate.x -= (mouseX-width/2) * (scale-oldScale)/(scale*oldScale);
  translate.y -= (mouseY-height/2) * (scale-oldScale)/(scale*oldScale);
}
