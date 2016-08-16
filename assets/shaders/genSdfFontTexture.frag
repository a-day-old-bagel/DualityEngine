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

float neighborVal(int blockOrder) {
    vec2 neighborCoord = getCoordFromCenter(blockOrder);
    vec2 neighborUv = vec2(uv.x + neighborCoord.x * pxAdvX, uv.y + neighborCoord.y * pxAdvY);
    return texture(tex, neighborUv).r;
}

float getDistSmoothCrude(int blockOrder) {
    vec2 neighborCoord = getCoordFromCenter(blockOrder);
    return sqrt(pow(neighborCoord.x, 2) + pow(neighborCoord.y, 2));
}

float getDistSharpCrude(int blockOrder) {
    vec2 neighborCoord = getCoordFromCenter(blockOrder);
    return max(abs(neighborCoord.x), abs(neighborCoord.y));
}

float getDistSmoothFine(int blockOrder) {
    vec2 neighborCoord = getCoordFromCenter(blockOrder);
    float darkness = 1.0 - neighborVal(blockOrder);
    float diagonalness = abs(dot(neighborCoord, vec2(1.0, 0.0)));
    if (diagonalness < 0.5) {
        diagonalness = abs(dot(neighborCoord, vec2(0.0, 1.0)));
    }
    darkness = mix(darkness, darkness * sqrt(2), diagonalness);
    return sqrt(pow(neighborCoord.x, 2) + pow(neighborCoord.y, 2) + darkness);
}

float nearestDist() {
    float dist = fuzzRadius;
    for (int i = 0; i < pow(fuzzRadius * 2 + 1, 2); ++i) {
        dist = mix(dist, min(dist, getDistSharpCrude(i)), step(0.001, neighborVal(i)));
    }
    return dist;
}

void main() {
    float dist = 1.0 - nearestDist() / fuzzRadius;
    fragColor = vec4(0.0, dist, 0.0, 1.0);
//    fragColor = vec4(0.0, step(0.9, dist), step(0.6, dist), 1.0);
//    fragColor = vec4(0.0, texture(tex, uv).r, texture(tex, uv).r, 1.0);
}
