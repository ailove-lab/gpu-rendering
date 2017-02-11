uniform mat4 mvp;
uniform vec3 col;
attribute vec2 pos;

varying vec3 color;

void main() {
    gl_Position = mvp * vec4(pos, 0.0, 1.0);
    color = col;
}