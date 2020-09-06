
PShader shader_data;
PShader shader_visible;
void init_render()
{
  
  shader_data = loadShader("dataFrag.glsl", "dataVert.glsl");
  shader_visible = loadShader("visFrag.glsl", "visVert.glsl");
}

PImage render_animation(int x, int y, int frames, int directions, PShader shader)
{
  PImage output = createImage(x * frames, y * directions, ARGB);
  PImage render = createImage(x,y, ARGB);
  for (int frame = 0; frame < frames; frame++)
  {
    character = loadOBJ(frame);
    for (int direction = 0; direction < directions; direction++)
    {
       render_character(render, direction, directions, shader);
       output.copy(render, 0, 0, x, y, x * frame, y * direction, x, y);
    }
  }
  return output;
}


PImage render_animation_data(int x, int y, int frames, int directions)
{
  return render_animation(x, y, frames, directions, shader_data);
}

PImage render_animation_visible(int x, int y, int frames, int directions)
{
  return render_animation(x, y, frames, directions, shader_visible);
}

void render_character(PImage output, int direction, int directions, PShader shader_)
{
  //((PGraphicsOpenGL)g).glEnable(GL.GL_BLEND);
  PGraphics render = createGraphics(output.width, output.height, P3D);
  render.hint(DISABLE_TEXTURE_MIPMAPS);;
  render.noSmooth();
  render.beginDraw();
  render.blendMode(REPLACE);
  render.hint(ENABLE_DEPTH_SORT);
  render.hint(ENABLE_DEPTH_MASK);
  render.hint(ENABLE_DEPTH_TEST);
  //render.hint(DISABLE_DEPTH_SORT);
  render.background(0, 0, 0, 0);
  render.shader(shader_);
  render.ortho();
  render.translate(64/2, 64/2);
  render.scale(-1,1,1);
  render.rotateX(-PI/6);
  render.rotateY(TWO_PI/(float)directions * direction);
  render.scale(8);
  render.shape(character,0,0);
  render.endDraw();
  output.copy(render, 0, 0, output.width, output.height, 0, 0, output.width, output.height);
}
