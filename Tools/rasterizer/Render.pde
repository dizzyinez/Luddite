
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
  JSONArray hitboxes = loadJSONObject("frame_data.json").getJSONArray("1");
  
  for (int frame = 0; frame < frames; frame++)
  {
    PShape character = loadOBJ(frame);
    for (int direction = 0; direction < directions; direction++)
    {
       render_character(render, direction, directions, shader, character);
       if ( frame < hitboxes.size() && hitboxes.getJSONObject(frame).getFloat("scale") > 0.01)
       {
       hitbox_calculations(hitboxes.getJSONObject(frame), direction, directions, render);
       }
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

void render_character(PImage output, int direction, int directions, PShader shader_, PShape character)
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
  render.scale(5);
  render.rotateX(-PI/6);
  render.rotateY(TWO_PI/(float)directions * direction);
  render.shape(character,0,0);
  render.endDraw();
  output.copy(render, 0, 0, output.width, output.height, 0, 0, output.width, output.height);
}

void hitbox_calculations(JSONObject hitbox, int direction, int directions, PImage output)
{
  PGraphics render = createGraphics(output.width, output.height, P3D);
  render.beginDraw();
  render.pushMatrix();
  render.blendMode(REPLACE);
  render.ortho();
  render.translate(64/2, 64/2);
  //render.scale(-1,1,1);
  render.scale(5);
  render.rotateX(PI/6);
  render.rotateY(TWO_PI/(float)directions * direction);
  render.translate(hitbox.getFloat("x"), hitbox.getFloat("y"), hitbox.getFloat("z"));
  float x = (render.screenX(0.0,0.0,0.0));
  float y = (render.screenY(0.0,0.0,0.0));
  fill(0x22552222);
  //render.lights();
  //render.sphere(hitbox.getFloat("scale"));
  render.popMatrix();
  render.ellipseMode(RADIUS);
  render.noStroke();
  //render.ellipse(x,y,hitbox.getFloat("scale")*5,hitbox.getFloat("scale")*5);
  //render.ellipse(x,y,10,10);
  render.endDraw();
  //output.blend(render, 0, 0, output.width, output.height, 0, 0, output.width, output.height, MULTIPLY);
  //hitbox
}
