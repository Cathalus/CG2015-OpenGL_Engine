#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

enum CameraType { PERSPECTIVE, ORTHO };

class Camera
{
public:
	Camera(const Camera& other) :
		_position(other._position), _perspective(other._perspective),
		_forward(other._forward), _up(other._up){};
	// Perspective Constructor
	Camera(const glm::vec3& pos, float fov, float aspect, float zNear, float zFar)
	{
		_perspective = glm::perspective(glm::radians(fov), aspect, zNear, zFar);
		_position = glm::vec3(pos.x,pos.y,-pos.z);
		_forward = glm::vec3(0, 0, -1);
		_up = glm::vec3(0, 1, 0);
		_type = PERSPECTIVE;
	}
	// Orthographic Constructor
	Camera(const glm::vec3& pos, float left, float right, float bottom, float top, float zNear, float zFar)
	{
		_perspective = glm::ortho<float>(left, right, bottom, top, zNear, zFar);
		_position = glm::vec3(pos.x, pos.y, -pos.z);
		_up = glm::vec3(0, 1, 0);
		_type = ORTHO;
	}

	virtual inline glm::mat4 getCameraProjection() const
	{
		glm::mat4 result(1.0f);
		switch (_type)
		{
			case ORTHO:
				result = glm::rotate(_perspective, glm::radians(90.0f), glm::vec3(0, 1, 0));
				result = glm::rotate(result, glm::radians(20.0f), glm::vec3(0, 0, 1));
				result = glm::translate(result, glm::vec3(0, 5, 0));
				return glm::translate(_perspective, glm::vec3(0, 0, 0)) *glm::lookAt(_position,
					_position + _forward,
					_up);
				//return result;
			case PERSPECTIVE:
				return _perspective * glm::lookAt(_position,
					_position + _forward,
					_up);
		}
	}
	virtual inline void setPosition(glm::vec3 pos) { _position = pos; };
	virtual inline glm::vec3 getPosition() { return glm::vec3(_position); };
	virtual inline void setForward(glm::vec3 fwd) { _forward = fwd; };
	virtual inline glm::vec3 getForward() { return glm::vec3(_forward); };
	virtual inline void setUp(glm::vec3 up) { _up = up; };
	virtual inline glm::vec3 getUp() { return glm::vec3(_up); };
private:
	CameraType _type;
	glm::mat4 _perspective;
	glm::vec3 _position;
	glm::vec3 _forward;
	glm::vec3 _up;
	glm::vec3 _direction;
};

#endif // CAMERA_H