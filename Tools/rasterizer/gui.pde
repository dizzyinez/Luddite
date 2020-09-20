DropdownList image_selection;
Textlabel selected_pixel_data_display;
Textfield frame_count;
Textfield frame_width;
Textfield frame_height;

PFont font;
void init_gui()
{
  font = createFont("courier new", 20);
  gui.addButton("Render")
    .setPosition(20,20)
    .setSize(100,25)
    .setMoveable(true);
    ;
  gui.addButton("Save")
    .setPosition(20,50)
    .setSize(100,25)
    .setMoveable(true);
    ;
  image_selection = gui.addDropdownList("Image")
  .setPosition(125, 20)
    .setSize(100,60)
  .setBackgroundColor(color(190))
  .setItemHeight(20)
  .setBarHeight(15)
  .addItem("Visible", 0)
  .addItem("Raw", 1);  
  ;
  selected_pixel_data_display = gui.addTextlabel("selected_pixel_data_display")
  .setText("")
  .setPosition(5, height-20)
  .setColorValue(0xffEEEEEE)
  .setFont(font)
  ;
  frame_count = gui.addTextfield("frames")
  .setPosition(230, 20)
  .setSize(30,20);
  ;
  frame_width = gui.addTextfield("width")
  .setPosition(265, 20)
  .setSize(30,20);
  ;
  frame_height = gui.addTextfield("height")
  .setPosition(300, 20)
  .setSize(30,20);
  ;
  
}

void Render()
{
  sprite_sheet_visible = render_animation_visible(Integer.parseInt(frame_width.getText()), Integer.parseInt(frame_height.getText()), Integer.parseInt(frame_count.getText()), 8);
  sprite_sheet_data = render_animation_data(Integer.parseInt(frame_width.getText()), Integer.parseInt(frame_height.getText()), Integer.parseInt(frame_count.getText()), 8);
}

void Save()
{
  sprite_sheet_data.save("anim.png");
}

void Image(int test)
{
  switch(test)
  {
    case 0:
  }
  println("test: " + test);
}
