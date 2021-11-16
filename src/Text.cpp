#include "Text.h"

Text::Text(string text, glm::vec3 position, glm::vec3 color, int size){
  _text = text;
  _position = position;
  _color = color;
  _size = size;
}

Text::~Text(){
  cout << "The objects has been deleted" << endl;
}

bool Text::init(){
  TTF_Init();
  _font = TTF_OpenFont("pixelplay.ttf", _size);
  _Color = {(Uint8)(_color.b * 255), (Uint8)(_color.g * 255), (Uint8)(_color.r * 255), 255};
  res_texture = TTF_RenderText_Blended(_font, _text.c_str(), _Color);

  float vertices[] = {
     0.0f,                   0.0f,                   0.0f,  0.0f, 0.0f,  // top right
     (float)res_texture->w,  0.0f,                   0.0f,  1.0f, 0.0f, // bottom right
     (float)res_texture->w,  (float)res_texture->h,  0.0f,  1.0f, 1.0f,// bottom left
     0.0f,                   (float)res_texture->h,  0.0f,  0.0f, 1.0f// top left
  };
  unsigned int indices[] = {  // note that we start from 0!
      0, 1, 3,   // first triangle
      1, 2, 3    // second triangle
  };

  glGenTextures(1, &_texture_id);
  glBindTexture(GL_TEXTURE_2D, _texture_id);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexImage2D(GL_TEXTURE_2D, // target
    0, // level, 0 = base, no minimap,
    GL_RGBA, // internalformat
    res_texture->w, // width
    res_texture->h, // height
    0, // border, always 0 in OpenGL ES
    GL_RGBA, // format
    GL_UNSIGNED_BYTE, // type
    res_texture->pixels);
  SDL_FreeSurface(res_texture);


  glGenBuffers(1, &_VBO);
  glGenBuffers(1, &_EBO);

  glBindBuffer(GL_ARRAY_BUFFER, _VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  glBindBuffer(GL_ARRAY_BUFFER, 0);

  if ((_vertexShader = create_shader("src/text.v.glsl", GL_VERTEX_SHADER))   == 0) return false;
  if ((_fragmentShader = create_shader("src/text.f.glsl", GL_FRAGMENT_SHADER)) == 0) return false;

  _shaderProgram = glCreateProgram();
  glAttachShader(_shaderProgram, _vertexShader);
  glAttachShader(_shaderProgram, _fragmentShader);
  glBindAttribLocation(_shaderProgram, 0, "aPos");
  glBindAttribLocation(_shaderProgram, 1, "aCoord");
  glLinkProgram(_shaderProgram);
  int success = false;

  glGetProgramiv(_shaderProgram, GL_LINK_STATUS, &success);

  if (!success) {
		cerr << "glLinkProgram:";
		print_log(_shaderProgram);
		return false;
	}
  return true;
}

void Text::draw(string text){
  if(text != "blabla"){
    res_texture = TTF_RenderText_Blended_Wrapped(_font, text.c_str(), _Color,200);
    glBindTexture(GL_TEXTURE_2D, _texture_id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, // target
      0, // level, 0 = base, no minimap,
      GL_RGBA, // internalformat
      res_texture->w, // width
      res_texture->h, // height
      0, // border, always 0 in OpenGL ES
      GL_RGBA, // format
      GL_UNSIGNED_BYTE, // type
      res_texture->pixels);
      float vertices[] = {
         0.0f,                   0.0f,                   0.0f,  0.0f, 0.0f,  // top right
         (float)res_texture->w,  0.0f,                   0.0f,  1.0f, 0.0f, // bottom right
         (float)res_texture->w,  (float)res_texture->h,  0.0f,  1.0f, 1.0f,// bottom left
         0.0f,                   (float)res_texture->h,  0.0f,  0.0f, 1.0f// top left
       };
    SDL_FreeSurface(res_texture);
    glBindBuffer(GL_ARRAY_BUFFER, _VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
  }


  glDisable(GL_DEPTH_TEST);
  glEnable(GL_TEXTURE_2D);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glUseProgram(_shaderProgram);

  glActiveTexture(GL_TEXTURE0);
	glUniform1i(glGetUniformLocation(_shaderProgram,"mytexture"), /*GL_TEXTURE*/0);
	glBindTexture(GL_TEXTURE_2D, _texture_id);

  glBindBuffer(GL_ARRAY_BUFFER, _VBO);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)(3 * sizeof(float)));

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _EBO);
  int size;  glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
  glDrawElements(GL_TRIANGLES, size, GL_UNSIGNED_INT, 0);
}

void Text::update(){
  glUseProgram(_shaderProgram);
  glm::mat4 m_model = glm::translate(glm::mat4(1.0f),_position);
  glm::mat4 m_projection = glm::ortho( 0.0f, 640.0f, 480.0f, 0.0f, 0.2f, 20.0f );
  //glm::mat4 m_projection = glm::perspective(45.0f, (float)(640)/(float)(480), 0.2f, 20.0f);
  glm::mat4 m_view = glm::lookAt(glm::vec3(0.0f, 0.0f,1.0f),glm::vec3(0.0f,0.0f,0.0f), glm::vec3(0.0,1.0,0.0));
  glm::mat4 m_transform = m_projection * m_view * m_model;
  glUniformMatrix4fv(glGetUniformLocation(_shaderProgram, "m_transform"), 1, GL_FALSE, glm::value_ptr(m_transform));
}

void Text::free(){
  cout << "The Text Resources are being deleted" << endl;
  TTF_Quit();
  glDeleteTextures(1, &_texture_id);
  glDeleteProgram(_shaderProgram);
  glDeleteShader(_vertexShader);
  glDeleteShader(_fragmentShader);
  glDeleteBuffers(1, &_EBO);
  glDeleteBuffers(1, &_VBO);
}
