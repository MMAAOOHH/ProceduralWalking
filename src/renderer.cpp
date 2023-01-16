#include "renderer.h"

#include "Mouse.h"


SpriteRenderer::SpriteRenderer()
	: vao_(0)
{
	unsigned int vbo;
	GLfloat vertices[] = {
		// position // texture coord
		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,

		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f
	};

	glGenVertexArrays(1, &this->vao_);
	glGenBuffers(1, &vbo);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindVertexArray(this->vao_);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void*)nullptr);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_ALWAYS);

}

SpriteRenderer::~SpriteRenderer()
{
	//todo: delete vao_
}

// const
// const material&, const glm::mat4& model, const glm::mat4& view
void SpriteRenderer::draw(const drawable& drawable, const glm::mat4& view)
{
	Material* material = drawable.material;
	Shader* shader = material->shader;

	shader->use();
	shader->set_vec3f("u_color", material->color);
	shader->set_mat4("u_model", drawable.get_model_transform());
	shader->set_mat4("u_view", view);
	shader->set_vec2f("u_resolution", drawable.size);
	material->bind();

	glBindVertexArray(this->vao_);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}




Renderer::Renderer(std::vector<GLuint> attributes, GLuint max_sprites)
	: vbo_(0), vao_(0), current_material_(nullptr), max_sprites_(max_sprites)
{
	attrib_size_ = 0;
    for (auto att : attributes) attrib_size_ += att;

    // create storage in gpu
    glGenVertexArrays(1, &vao_);
    glGenBuffers(1, &vbo_);

    glBindVertexArray(vao_);

    // set up vbo data inside gpu
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);

    auto stride = 0ull;
    for (auto i = 0ull; i < attributes.size(); ++i)
    {
        // set up attributes to map vbo in gpu
        glVertexAttribPointer(i, attributes[i], GL_FLOAT, GL_FALSE, attrib_size_ * sizeof(GLfloat), (GLvoid*)stride);
        glEnableVertexAttribArray(i);
        stride += sizeof(GLfloat) * attributes[i];
    }
}

Renderer::~Renderer()
{
	if (vbo_)
        glDeleteBuffers(1, &vbo_);
    if (vao_)
        glDeleteBuffers(1, &vao_);

    vbo_ = 0;
    vao_ = 0;
}

void Renderer::begin()
{
	current_material_ = nullptr;
}

void Renderer::end()
{
	flush();
}

void Renderer::draw(drawable& drawable_struct)
{
	/*
	rect dest_rect = { drawable_struct.position.x, drawable_struct.position.y , drawable_struct.size.x, drawable_struct.size.y };
	// handle texture rect some other way?
	rect src_rect = { 0,0, drawable_struct.size.x, drawable_struct.size.y };
	auto& material = drawable_struct.material;

	if ((this->buffer_.size() >= this->max_sprites_ * 6 * this->attrib_size_ || !current_material_) ||
		this->current_material_->id != material.id)
	{
		this->flush();
		this->current_material_ = &material;
	}

	// translate to fractions of the image dimensions
	auto norm_src = src_rect;
	auto img_w = material.texture.width;
	auto img_h = material.texture.height;

	norm_src.x /= img_w;
	norm_src.y /= img_h;
	norm_src.w /= img_w;
	norm_src.h /= img_h;

	// first triangle

	// bot left
	buffer_.push_back(dest_rect.x);
	buffer_.push_back(dest_rect.y + dest_rect.h);
	// texture
	buffer_.push_back(norm_src.x);
	buffer_.push_back(norm_src.y + norm_src.h);

	// top right
	buffer_.push_back(dest_rect.x + dest_rect.w);
	buffer_.push_back(dest_rect.y);
	// texture
	buffer_.push_back(norm_src.x + norm_src.w);
	buffer_.push_back(norm_src.y);

	// top left
	buffer_.push_back(dest_rect.x);
	buffer_.push_back(dest_rect.y);
	// texture
	buffer_.push_back(norm_src.x);
	buffer_.push_back(norm_src.y);

	// second triangle

	// bot left
	buffer_.push_back(dest_rect.x);
	buffer_.push_back(dest_rect.y + dest_rect.h);
	// texture
	buffer_.push_back(norm_src.x);
	buffer_.push_back(norm_src.y + norm_src.h);

	// bot right
	buffer_.push_back(dest_rect.x + dest_rect.w);
	buffer_.push_back(dest_rect.y + dest_rect.h);
	// texture
	buffer_.push_back(norm_src.x + norm_src.w);
	buffer_.push_back(norm_src.y + norm_src.h);

	// top right
	buffer_.push_back(dest_rect.x + dest_rect.w);
	buffer_.push_back(dest_rect.y);
	// texture
	buffer_.push_back(norm_src.x + norm_src.w);
	buffer_.push_back(norm_src.y);


	material.shader.set_vec2f("u_resolution", drawable_struct.size);
	//material.shader.set_mat4("u_model", drawable_struct.get_model_transform());
	*/
}

void Renderer::flush()
{
	if (this->buffer_.empty()) 
		return;

	if(!this->current_material_)
	{
		this->buffer_.clear();
		return;
	}

	this->current_material_->compile();
	this->current_material_->bind();

    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * this->buffer_.size(), this->buffer_.data(), GL_STATIC_DRAW);

	// draw triangle
	glDrawArrays(GL_TRIANGLES, 0, this->buffer_.size() / attrib_size_);

	// clear buffer for next cycle
	buffer_.clear();
}
