#include "Cube.h"

extern float delta;

Cube::Cube(string name, glm::vec3 position, float angle, glm::vec3 color){
  _position = position;
  _angle = angle;
  _color = color;
  teste = name;
}

Cube::~Cube(){
  cout << "The object has been deleted" << endl;
}

bool Cube::init(float vertices[], int bytesize){
  unsigned int indices[] = {
    // front
     0,  1,  2,
     2,  3,  0,
    // top
     4,  5,  6,
     6,  7,  4,
    // back
     8,  9, 10,
    10, 11,  8,
    // bottom
    12, 13, 14,
    14, 15, 12,
    // left
    16, 17, 18,
    18, 19, 16,
    // right
    20, 21, 22,
    22, 23, 20,
  };

  SDL_Surface* res_texture = IMG_Load("image.jpg");
	if (res_texture == NULL) {
		cerr << "IMG_Load: " << SDL_GetError() << endl;
		return false;
	}

	glGenTextures(1, &_texture_id);
	glBindTexture(GL_TEXTURE_2D, _texture_id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, // target
		0, // level, 0 = base, no minimap,
		GL_RGB, // internalformat
		res_texture->w, // width
		res_texture->h, // height
		0, // border, always 0 in OpenGL ES
		GL_RGB, // format
		GL_UNSIGNED_BYTE, // type
		res_texture->pixels);
	SDL_FreeSurface(res_texture);

  glGenBuffers(1, &_VBO);
  glGenBuffers(1, &_EBO);

  glBindBuffer(GL_ARRAY_BUFFER, _VBO);
  glBufferData(GL_ARRAY_BUFFER, bytesize, vertices, GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  glBindBuffer(GL_ARRAY_BUFFER, 0);

	if ((_vertexShader = create_shader("src/cube.v.glsl", GL_VERTEX_SHADER))   == 0) return false;
	if ((_fragmentShader = create_shader("src/cube.f.glsl", GL_FRAGMENT_SHADER)) == 0) return false;

  _shaderProgram = glCreateProgram();
  glAttachShader(_shaderProgram, _vertexShader);
  glAttachShader(_shaderProgram, _fragmentShader);
  glBindAttribLocation(_shaderProgram, 0, "aPos");
  glBindAttribLocation(_shaderProgram, 1, "aCoord");
  glBindAttribLocation(_shaderProgram, 2, "aNormal");
  glLinkProgram(_shaderProgram);

  int success = false;

  glGetProgramiv(_shaderProgram, GL_LINK_STATUS, &success);

  if (!success) {
		cerr << "glLinkProgram:";
		print_log(_shaderProgram);
		return false;
	}

  glUseProgram(_shaderProgram);
  glUniform3fv(glGetUniformLocation(_shaderProgram,"OurColor"),1, &_color[0]);
  cout << "The resources has been initialized" << endl;
  return true;
}

void Cube::draw(){
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);
  glUseProgram(_shaderProgram);

  glActiveTexture(GL_TEXTURE0);
	glUniform1i(glGetUniformLocation(_shaderProgram,"mytexture"), /*GL_TEXTURE*/0);
	glBindTexture(GL_TEXTURE_2D, _texture_id);

  glBindBuffer(GL_ARRAY_BUFFER, _VBO);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(5 * sizeof(float)));

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _EBO);
  int size;  glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
  glDrawElements(GL_TRIANGLES, size, GL_UNSIGNED_INT, 0);

  //glDrawArrays(GL_TRIANGLES, 0, 3);
}

void Cube::update(glm::mat4 view, glm::mat4 projection, glm::vec3 lightpos){
  //float deltaTime = delta / 1000.0;
  float angle = SDL_GetTicks() / 1000.0 * 40;

  glm::mat4 m_model = glm::translate(glm::mat4(1.0f),_position) * glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(0.0,1.0,0.0));

  //glm::mat4 m_transform = projection * view * m_model;

  //_time += SDL_GetTicks() / 1000.0 * 0.05;
  _time += (delta / 1000.0) / 2;


  glUseProgram(_shaderProgram);
  glUniformMatrix4fv(glGetUniformLocation(_shaderProgram, "m_model"), 1, GL_FALSE, glm::value_ptr(m_model));
  glUniformMatrix4fv(glGetUniformLocation(_shaderProgram, "m_view"), 1, GL_FALSE, glm::value_ptr(view));
  glUniformMatrix4fv(glGetUniformLocation(_shaderProgram, "m_projection"), 1, GL_FALSE, glm::value_ptr(projection));
  glUniform1f(glGetUniformLocation(_shaderProgram, "time"),_time);
  glUniform3fv(glGetUniformLocation(_shaderProgram,"LightPos"),1, &lightpos[0]);
}

void Cube::free(){
  cout << "The resources are being deleted" << endl;
  glDeleteTextures(1, &_texture_id);
  glDeleteProgram(_shaderProgram);
  glDeleteShader(_vertexShader);
  glDeleteShader(_fragmentShader);
  glDeleteBuffers(1, &_EBO);
  glDeleteBuffers(1, &_VBO);
}
