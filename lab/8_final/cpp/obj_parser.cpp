#include "obj_parser.h"
vertex_attr::vertex_attr(GLfloat _x, GLfloat _y, GLfloat _z) {
	vertex_attr::x = _x;
	vertex_attr::y = _y;
	vertex_attr::z = _z;
}
vertex_attr::vertex_attr() {}
obj_parser::obj_parser(string filePath)
{
	ifstream ifs(filePath, ios::in);
	if (!ifs.is_open())
	{
		throw "未成功讀取檔案";
	}
	stringstream ss;
	ss << ifs.rdbuf();
	input = ss.str();
	//std::cout << input;
	ifs.close();
	this->load();
	this->findboudingbox();
	this->scale();
	this->translate();
	/*cout << "mid_x:" << mid_x << endl;
	cout << "mid_y:" << mid_y << endl;
	cout << "mid_z:" << mid_z << endl;
	cout << "scale_factor:" << scale_factor << endl;*/
}
void obj_parser::scale() 
{
	for (auto it = vertex_origin.begin(); it != vertex_origin.end(); ++it)
	{
		//縮放
		(*it).x /= scale_factor;
		(*it).y /= scale_factor;
		(*it).z /= scale_factor;
	}
	
}
void obj_parser::translate()
{
	vertex.clear();
	for (auto it = vertex_origin.begin(); it != vertex_origin.end(); ++it)
	{
		vertex_attr v((*it).x + mid_x,
			 (*it).y + mid_y,
			(*it).z + mid_z);
		vertex.push_back(v);
	}
	bonding_point.clear();
	for (auto it = bonding_point_origin.begin(); it != bonding_point_origin.end(); ++it)
	{
		vertex_attr v((*it).x + mid_x,
			(*it).y + mid_y,
			(*it).z + mid_z);
		bonding_point.push_back(v);
	}
	
}
void obj_parser::load()
{
	for (; pos < input.length(); pos++) {
		if (pos < input.length()) {
			pos++;
			string::size_type next = pos;
			pos--;
			//cout << "now:" << input[pos]<<endl;
			if (input[pos] == 'v' && input[next] == ' ')
			{
				pos++;
				skipWhiteSpace();
				nextfloat();
			}
			if (input[pos] == 'f' && input[next] == ' ')
			{
				pos++;
				skipWhiteSpace();
				nextindex();
			}
		}
	}
}
void obj_parser::nextfloat()
{
	unsigned int index = 0;
	string temp[] = { "","","" };
	string E[] = { "","","" };
	while (1)
	{
		if (input[pos] <= '9' && input[pos] >= '0') {
			temp[index] += input[pos];
		}
		else if (input[pos] == '.') {
			temp[index] += input[pos];
		}
		else if (input[pos] == '-') {
			temp[index] += input[pos];
		}
		else if (input[pos] == ' ') {
			index++;
		}
		//科學記號
		else if (input[pos] == 'E') {
			pos++;
			pos++;
			while (1) 
			{
				if (input[pos] <= '9' && input[pos] >= '0')
				{
					E[index] += input[pos];
					pos++;
					string::size_type next = pos;
					pos--;
					if (input[next] == ' ' || input[next] == '\n' || input[next] == '\t')
					{
						break;
					}
				}
			}
		}
		else if (index == 2) {

			vertex_attr v;
			if (E[0] != "") {
				float in = stof(temp[0]);
				int range = stoll(E[0]);
				for (int i = 0; i < range; i++)
				{
					in /= 10;
				}
				v.x = in;
			}
			else 
			{
				v.x = stof(temp[0]);
			}

			if (E[1] != "") {
				float in = stof(temp[1]);
				int range = stoll(E[1]);
				for (int i = 0; i < range; i++)
				{
					in /= 10;
				}
				v.y = in;
			}
			else
			{
				v.y = stof(temp[1]);
			}

			if (E[2] != "") {
				float in = stof(temp[1]);
				int range = stoll(E[2]);
				for (int i = 0; i < range; i++)
				{
					in /= 10;
				}
				v.z = in;
			}
			else
			{
				v.z = stof(temp[2]);
			}
			vertex_origin.push_back(v);
			break;
		}
		pos++;
	}
}
void obj_parser::nextindex()
{
	int index = 0;
	string temp[] = { "","","" };
	while (1)
	{
		if (input[pos] <= '9' && input[pos] >= '0') {
			temp[index] += input[pos];
		}
		else if (input[pos] == '.') {
			temp[index] += input[pos];
		}
		else if (input[pos] == '-') {
			temp[index] += input[pos];
		}
		else if (input[pos] == ' ') {
			index++;
		}
		else if (index == 2) {
			surface.push_back(stoll(temp[0]) - 1);
			surface.push_back(stoll(temp[1]) - 1);
			surface.push_back(stoll(temp[2]) - 1);
			break;
		}
		pos++;
	}
}
void obj_parser::skipWhiteSpace()
{
	while (input[pos] == ' ' || input[pos] == '\n' || input[pos] == '\t')
	{
		if (pos < input.length())
		{
			pos++;
		}
	}
}
void obj_parser::findboudingbox()
{
	GLfloat x_max= vertex_origin[0].x, x_min= vertex_origin[0].x, y_max= vertex_origin[0].y, y_min= vertex_origin[0].y, z_max= vertex_origin[0].z, z_min= vertex_origin[0].z;
	for (auto it = vertex_origin.begin(); it != vertex_origin.end(); ++it) {
		
		if ((*it).x > x_max) {
			x_max = (*it).x;
		}
		if ((*it).x < x_min) {
			x_min = (*it).x;
		}
		if ((*it).y > y_max) {
			y_max = (*it).y;
		}
		if ((*it).y < y_min) {
			y_min = (*it).y;
		}
		if ((*it).z > z_max) {
			z_max = (*it).z;
		}
		if ((*it).z < z_min) {
			z_min = (*it).z;
		}
	}
	
	width = x_max - x_min;
	height = y_max - y_min;
	depth = z_max - z_min;
	
	//找縮放係數
	
	scale_factor = fabs(height);
	if (scale_factor > 0.7) 
	{
		scale_factor = scale_factor / 0.7;
		width /= scale_factor;
		height /= scale_factor;
		depth /= scale_factor;
		x_max /= scale_factor;
		x_min /= scale_factor;
		y_max /= scale_factor;
		y_min /= scale_factor;
		z_max /= scale_factor;
		z_min /= scale_factor;

	}
	/*cout << "x:" << x_max << "," << x_min  << endl;
	cout << "y:" << y_max << "," << y_min << endl;
	cout << "z:" << z_max << "," << z_min << endl;
	cout << "width:" << width << endl;
	cout << "height:" << height << endl;
	cout << "depth:" << depth << endl;*/
	//找位移
	mid_x = 0.0f - (width / 2 + x_min);
	mid_y = 0.0f - (height / 2 + y_min);
	mid_z = 0.0f - (depth / 2 + z_min);
	/*cout << "x:" << x_max << "," << x_min << "," << mid_x << endl;
	cout << "y:" << y_max << "," << y_min << "," << mid_y << endl;
	cout << "z:" << z_max << "," << z_min << "," << mid_z << endl;
	cout << "width:" << width << endl;
	cout << "height:" << height << endl;
	cout << "depth:" << depth << endl;*/

	//外
	//左下
	vertex_attr v(x_min, y_min, z_max);
	bonding_point_origin.push_back(v);
	//右下
	v.x = x_max;
	v.y = y_min;
	v.z = z_max;
	bonding_point_origin.push_back(v);
	//右上
	v.x = x_max;
	v.y = y_max;
	v.z = z_max;
	bonding_point_origin.push_back(v);
	//左上
	v.x = x_min;
	v.y = y_max;
	v.z = z_max;
	bonding_point_origin.push_back(v);
	//內
	//左下
	v.x = x_min;
	v.y = y_min;
	v.z = z_min;
	bonding_point_origin.push_back(v);
	//右下
	v.x = x_max;
	v.y = y_min;
	v.z = z_min;
	bonding_point_origin.push_back(v);
	//右上
	v.x = x_max;
	v.y = y_max;
	v.z = z_min;
	bonding_point_origin.push_back(v);
	//左上
	v.x = x_min;
	v.y = y_max;
	v.z = z_min;
	bonding_point_origin.push_back(v);

}

