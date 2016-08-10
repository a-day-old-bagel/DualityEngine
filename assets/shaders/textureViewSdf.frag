#version 130

in vec2 uv;
out vec4 fragColor;
uniform sampler2D tex;
uniform float pxAdvX;
uniform float pxAdvY;
uniform int fuzzRadius;

vec2 getCoordFromBottomLeft(int blockOrder) {
    return vec2(float(blockOrder % (fuzzRadius * 2 + 1)), float(blockOrder / (fuzzRadius * 2 + 1)));
}

vec2 getCoordFromCenter(int blockOrder) {
    vec2 fromBottomLeft = getCoordFromBottomLeft(blockOrder);
    return vec2(fromBottomLeft.x - fuzzRadius, fromBottomLeft.y - fuzzRadius);
}

float neighborVal(int blockOrder, vec2 uv) {
    vec2 neighborCoord = getCoordFromCenter(blockOrder);
    vec2 neighborUv = vec2(uv.x + neighborCoord.x * pxAdvX, uv.y + neighborCoord.y * pxAdvY);
    return texture(tex, neighborUv).r;
}

float getDistFrom(int blockOrder) {
    vec2 neighborCoord = getCoordFromCenter(blockOrder);
    return sqrt(pow(neighborCoord.x, 2) + pow(neighborCoord.y, 2));
}

float nearestDist(vec2 uv) {
    float dist = fuzzRadius;
    for (int i = 0; i < pow(fuzzRadius * 2, 2); ++i) {
        dist = mix(dist, min(dist, getDistFrom(i)), step(1.0, neighborVal(i, uv)));
    }
    return dist;
}

void main() {
    fragColor = vec4(0.0, 1.0 - nearestDist(uv) / fuzzRadius, 0.0, 1.0);
}
