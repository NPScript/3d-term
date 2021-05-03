#include <curses.h>
#include <array>
#include <math.h>
#include <iostream>
#include <unistd.h>

const double PI = 3.141592653589793238463;
#define NULL_VECTOR Vector<3>({0, 0, 0})

std::array<char, 10> depth = {'.', '.', '.', 'c', 'o', 'e', 's', '0', 'O', '@'};

template<unsigned D>
class Vector {
	private:
		std::array<double, D> data;
	public:
		Vector(const std::array<double, D> & array) : data(array) {}
		Vector() = default;
		~Vector() = default;
		double & operator[](unsigned i) {
			return data[i];
		}

		double length() {
			double l = 0;
			for (double i : this->data) {
				l += i * i;
			}

			l = sqrt(l);
			return l;
		}

		std::array<double, D> & get_data() {
			return this->data;
		}

		void operator+=(Vector<D> & vec) {
			for (int i = 0; i < this->data.size(); ++i) {
				this->data[i] += vec[i];
			}
		}

		Vector<D> operator-(Vector<D> & vec) {
			Vector<D> out;
			for (int i = 0; i < D; ++i) {
				out[i] = this->data[i] - vec[i];
			}

			return out;
		}

		Vector<D> normalized() {
			Vector<D> out(*this);

			double l = this->length();

			for (double & i : out.get_data()) {
				i /= l;
			}

			return out;
		}
};

Vector<2> zero;



template<int C, int R>
class Matrix {
	private:
		std::array<Vector<R>, C> data;
	public:
		Matrix(const std::array<Vector<R>, C> & array) : data(array) {}
		Matrix() = default;
		~Matrix() = default;
		Vector<R> & operator[](unsigned i) {
			return data[i];
		}

		Matrix<C, R> operator*(Matrix<C, R> & mat) {
			Matrix<C, R> out;

			for (int i = 0; i < C * R; ++i) {
				out[i % C][i / R] = 0;
			}

			for (int i = 0; i < C * R; ++i) {
				for (int j = 0; j < C; ++j) {
					out[i % C][i / R] += (*this)[j][i / C] * mat[i % C][j];
				}
			}

			return out;
		}

		Vector<R> operator*(Vector<C> & vec) {
			Vector<R> out;

			for (int i = 0; i < R; ++i) {
				out[i] = 0;
				for (int j = 0; j < C; ++j) {
					out[i] += (*this)[j][i] * vec[j];
				}
			}

			return out;
		}
};

struct Line {
	Vector<3> from;
	Vector<3> to;
	struct Line operator*(Matrix<3, 3> & mat) {
		struct Line line;
		line.from = mat * from;
		line.to = mat * to;
		return line;
	}
};

Matrix<3, 2> proj;
Matrix<3, 3> rot;

void print_vector(Vector<3> & vec) {
	Vector<2> projection;
	projection = proj * vec;
	projection[1] *= -1;
	projection += zero;
	char p;

	p = depth[vec[2] / 5 + 5];
	
	mvprintw((int) projection[1], (int) projection[0], "%c", p);
}

void draw_line(Vector<3> from, Vector<3> to) {
	Vector<3> start = from;
	Vector<3> direction = to - from;
	int m = (int) direction.length();
	direction = direction.normalized();

	for (int i = 0; i < m; ++i) {
		print_vector(start);
		start += direction;
	}
}

void draw_cube(double side) {
	struct Line line;
	side /= 2;
	line.from = Vector<3>({-side, -side, -side});
	line.to = Vector<3>({side, -side, -side});
	line = line * rot;
	draw_line(line.from, line.to);

	line.from = Vector<3>({-side, -side, -side});
	line.to = Vector<3>({-side, -side, side});
	line = line * rot;
	draw_line(line.from, line.to);

	line.from = Vector<3>({side, -side, -side});
	line.to = Vector<3>({side, -side, side});
	line = line * rot;
	draw_line(line.from, line.to);

	line.from = Vector<3>({-side, -side, side});
	line.to = Vector<3>({side, -side, side});
	line = line * rot;
	draw_line(line.from, line.to);

	line.from = Vector<3>({-side, side, -side});
	line.to = Vector<3>({side, side, -side});
	line = line * rot;
	draw_line(line.from, line.to);

	line.from = Vector<3>({-side, side, -side});
	line.to = Vector<3>({-side, side, side});
	line = line * rot;
	draw_line(line.from, line.to);

	line.from = Vector<3>({side, side, -side});
	line.to = Vector<3>({side, side, side});
	line = line * rot;
	draw_line(line.from, line.to);

	line.from = Vector<3>({-side, side, side});
	line.to = Vector<3>({side, side, side});
	line = line * rot;
	draw_line(line.from, line.to);

	line.from = Vector<3>({-side, side, -side});
	line.to = Vector<3>({-side, -side, -side});
	line = line * rot;
	draw_line(line.from, line.to);

	line.from = Vector<3>({-side, side, side});
	line.to = Vector<3>({-side, -side, side});
	line = line * rot;
	draw_line(line.from, line.to);

	line.from = Vector<3>({side, side, side});
	line.to = Vector<3>({side, -side, side});
	line = line * rot;
	draw_line(line.from, line.to);

	line.from = Vector<3>({side, side, -side});
	line.to = Vector<3>({side, -side, -side});
	line = line * rot;
	draw_line(line.from, line.to);

	line.from = Vector<3>({side, side, side});
	line.to = Vector<3>({side, -side, side});
	line = line * rot;
	draw_line(line.from, line.to);
}

int main() {
	initscr();
	curs_set(0);
	
	int x, y;
	getmaxyx(stdscr, y, x);
	zero[0] = x/2;
	zero[1] = y/2;
	
	proj[0][0] = 2;
	proj[0][1] = 0;
	proj[1][0] = 0;
	proj[1][1] = 1;
	proj[2][0] = 0.5;
	proj[2][1] = -0.5;

	for (int k = 0; 1; ++k) {
		clear();

		getmaxyx(stdscr, y, x);
		zero[0] = x/2;
		zero[1] = y/2;

		rot[0][0] = cos(PI / 64 * k);
		rot[0][1] = 0;
		rot[0][2] = sin(PI / 64 * k);
		rot[1][0] = 0;
		rot[1][1] = 1;
		rot[1][2] = 0;
		rot[2][0] = sin(PI / 64 * k);
		rot[2][1] = 0;
		rot[2][2] = -cos(PI / 64 * k);

		draw_cube(20);

		refresh();
		usleep(50000);
	}

	getch();
	endwin();
	std::cout << x << ", " << y << "\n" << zero[0] << ", " << zero[1] << '\n';
}
