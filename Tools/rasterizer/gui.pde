DropdownList image_selection;
Textlabel selected_pixel_data_display;
void init_gui()
{
  gui.addButton("Save")
    .setPosition(20,50)
    .setSize(100,25)
    .setMoveable(true);
    ;
  gui.addButton("Render")
    .setPosition(20,20)
    .setSize(100,25)
    .setMoveable(true);
    ;
  image_selection = gui.addDropdownList("Image")
  .setPosition(125, 20)
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
  .setFont(createFont("courier new", 20))
  ;
  
}

void Render()
{
  sprite_sheet_visible = render_animation_visible(64, 64, 22, 8);
  sprite_sheet_data = render_animation_data(64,64,22, 8);
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
