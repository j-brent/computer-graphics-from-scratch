# Computer Graphics from Scratch

- https://www.gabrielgambetta.com/computer-graphics-from-scratch/


## Light


### Abstract Base Class
```c++
    class Light
    {
      public:
        virtual float intensity(const Position3D& P, const Vector3D& N) const  = 0;
    };
  
    class AmbientLight : public Light
    {
      public:
        AmbientLight(float intensity) : intensity_{intensity} {}

        float intensity(const Position3D&, const Vector3D&) const override
        {
            return intensity_;
        }

      private:
        float intensity_;
    };

    class PointLight : public Light
    {
      public:
        PointLight(float intensity, Position3D pos) : position{pos}, intensity_{intensity} {}

        float intensity(const Position3D& P = {}, const Vector3D& N = {}) const override
        {
            Vector3D L = position - P;
            float dotLN = dot(L, N);
            if (dotLN > 0)
            {
                return intensity_ * dotLN / (length(L) * length(N));
            }
            return 0;
        }

      private:
        Position3D position;
        float intensity_;
    };

    class DirectionalLight : public Light
    {
      public:
        DirectionalLight(float intensity, Vector3D dir) : direction{dir}, intensity_{intensity} {}

        float intensity(const Position3D& P = {}, const Vector3D& N = {}) const override
        {
            float dotLN = dot(direction, N);
            if (dotLN > 0)
            {
                return intensity_ * dotLN / (length(direction) * length(N));
            }
            return 0;
        }
      private:
        Vector3D direction;
        float intensity_;
    };
```

dosen't work

```c++
std::vector<Light> lights{
      cgfs::AmbientLight{0.2f},
      cgfs::PointLight{0.6f, {2, 1, 0}},
      cgfs::DirectionalLight{0.2f, {1, 4, 4}}
};
```

leads to 
```sh
...
[build] E:\dev\computer-graphics-from-scratch\src\demo.cpp(61,25): error C2259: 'cgfs::Light': cannot instantiate abstract class ...
[build]       E:\dev\computer-graphics-from-scratch\src\light.h(10,11):
[build]       see declaration of 'cgfs::Light'
[build]       E:\dev\computer-graphics-from-scratch\src\demo.cpp(61,25):
[build]       due to following members:
[build]       E:\dev\computer-graphics-from-scratch\src\demo.cpp(61,25):
[build]       'float cgfs::Light::intensity(const cgfs::Position3D &,const cgfs::Vector3D &) const': is abstract
[build]       E:\dev\computer-graphics-from-scratch\src\light.h(13,23):
[build]       see declaration of 'cgfs::Light::intensity'
...
```