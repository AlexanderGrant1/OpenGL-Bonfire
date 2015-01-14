//Shadow matrix functions
//I slightly modified this so that you could provide a model position and it would then calculate a shadow matrix based on a hard-coded positional light source
#include "Shadow.h"


void Shadow::shadow_matrix(glm::vec4 position, glm::vec4 pl, glm::mat4 &shadow_proj)
{
	//Calculate light position relative to object
	glm::vec4 light_pos = glm::vec4(0, 0, -5.5, 0);
	glm::vec4 lt = light_pos - position;
	// dot product of normal and light position/direction
	GLfloat rdotl;
	rdotl = pl.x*lt.x + pl.y*lt.y + pl.z*lt.z + pl.w*lt.w;

	shadow_proj[0][0] = -pl.x*lt.x + rdotl;
	shadow_proj[0][1] = -pl.x*lt.y;
	shadow_proj[0][2] = -pl.x*lt.z;
	shadow_proj[0][3] = -pl.x*lt.w;
	shadow_proj[1][0] = -pl.y*lt.x;
	shadow_proj[1][1] = -pl.y*lt.y + rdotl;
	shadow_proj[1][2] = -pl.y*lt.z;
	shadow_proj[1][3] = -pl.y*lt.w;
	shadow_proj[2][0] = -pl.z*lt.x;
	shadow_proj[2][1] = -pl.z*lt.y;
	shadow_proj[2][2] = -pl.z*lt.z + rdotl;
	shadow_proj[2][3] = -pl.z*lt.w;
	shadow_proj[3][0] = -pl.w*lt.x;
	shadow_proj[3][1] = -pl.w*lt.y;
	shadow_proj[3][2] = -pl.w*lt.z;
	shadow_proj[3][3] = -pl.w*lt.w + rdotl;

}