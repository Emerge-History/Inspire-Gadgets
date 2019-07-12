double threeway_max(double a, double b, double c) {
  return max(a, max(b, c));
}

double threeway_min(double a, double b, double c) {
  return min(a, min(b, c));
}
void rgbToCmyk(uint8_t *getRgb, int *cmyk) {
  float rgb[3] = {
    float(getRgb[0]),
    float(getRgb[1]),
    float(getRgb[2])
  };
  float black = min(min(1 - rgb[0] / 255, 1 - rgb[1] / 255), 1 - rgb[2] / 255);
  if (black != 1) {
    cmyk[0] = int(((1 - rgb[0] / 255 - black) / (1 - black)) * 100);
    cmyk[1] = int(((1 - rgb[1] / 255 - black) / (1 - black)) * 100);
    cmyk[2] = int(((1 - rgb[2] / 255 - black) / (1 - black)) * 100);
    cmyk[3] = int(black * 100);
  } else {
    cmyk[0] = 0;
    cmyk[1] = 0;
    cmyk[2] = 0;
    cmyk[3] = 100;
  }
}

void rgbToHsv(uint8_t *getRgb, int *hsv) {
  double rd = (double) getRgb[0] / 255;
  double gd = (double) getRgb[1]  / 255;
  double bd = (double) getRgb[2]  / 255;
  double max = threeway_max(rd, gd, bd), min = threeway_min(rd, gd, bd);
  double h, s, v = max;
  double d = max - min;
  s = max == 0 ? 0 : d / max;
  if (max == min) {
    h = 0; // achromatic
  } else {
    if (max == rd) {
      h = (gd - bd) / d + (gd < bd ? 6 : 0);
    } else if (max == gd) {
      h = (bd - rd) / d + 2;
    } else if (max == bd) {
      h = (rd - gd) / d + 4;
    }
    h /= 6;
  }
  hsv[0] = int(h * 360);
  hsv[1] = int(s * 100);
  hsv[2] = int(v * 100);

}
