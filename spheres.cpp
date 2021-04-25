#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <cmath>
#include <vector>
#include <array>
#include <cstdlib>
#include <typeinfo>

// Copyright 2021 Mussaiyn Magzhan 170107185@stu.sdu.edu.kz
// Copyright 2021 Urzhan Rysbek 180107019@stu.sdu.edu.kz
// Copyright 2021 Yerden Sarkulov 170107065@stu.sdu.edu.kz

using namespace std;

class Sphere
{
public:
    float mass, radius, posx, posy, posz, vx, vy, vz;
    string name;
    int bounces;
    string printSphere(void);
    Sphere(float mass, float radius, float posx, float posy, float posz, float vx, float vy, float vz, string name)
    {
        this->mass = mass;
        this->radius = radius;
        this->posx = posx;
        this->posy = posy;
        this->posz = posz;
        this->vx = vx;
        this->vy = vy;
        this->vz = vz;
        this->name = name;
        this->bounces = 0;
    }
    Sphere()
    {
        this->mass = 0;
        this->radius = 0;
        this->posx = 0;
        this->posy = 0;
        this->posz = 0;
        this->vx = 0;
        this->vy = 0;
        this->vz = 0;
        this->name = "sphere";
        this->bounces = 0;
    }
};
ofstream myfile;
string Sphere::printSphere(void)
{
    cout << "Sphere " << name << " has mass " << mass << ", radius " << radius
         << ". Its initial position is (" << posx << "," << posy << "," << posz << ")"
         << " and its initial velocity is (" << vx << "," << vy << "," << vz << ") bounces=" << bounces << endl;
    return "";
}

vector<Sphere> readInputFile(string filename)
{
    std::ifstream file(filename.c_str());
    char c;
    int count = 0;
    string line;

    vector<Sphere> spheres;
    int counter = 0;
    while (getline(file, line))
    {
        stringstream stream(line);
        float mass, radius, posx, posy, posz, vx, vy, vz;
        string name;
        stream >> mass >> radius >> posx >> posy >> posz >> vx >> vy >> vz >> name;
        spheres.push_back(Sphere(mass, radius, posx, posy, posz, vx, vy, vz, name));
    }
    if (file.eof())
    {
        std::cout << "" << endl;
    }
    else
        std::cout << "\ncannot find file\n";
    file.close();
    return spheres;
}

void writeOutputFile(string str)
{
    ofstream myfile;
    myfile.open("example1.txt");
    myfile << str;
    myfile.close();
}

float calculateEnergy(vector<Sphere> spheres)
{
    vector<float> spheres_velocity;
    for (int i = 0; i < spheres.size(); ++i)
    {
        float sphere_velocity = (pow(spheres[i].vx, 2) + pow(spheres[i].vy, 2) + pow(spheres[i].vz, 2));
        spheres_velocity.push_back(sphere_velocity);
    }

    float energy;
    for (int i = 0; i < spheres.size(); ++i)
    {
        energy += (spheres[i].mass * spheres_velocity[i]) / 2;
    }
    return energy;
}

void calculateMomentum(vector<Sphere> spheres)
{
    float momentumx=0, momentumy=0, momentumz=0;
    for (int i = 0; i < spheres.size(); ++i){
        momentumx += (spheres[i].mass * spheres[i].vx);
        momentumy += (spheres[i].mass * spheres[i].vy);
        momentumz += (spheres[i].mass * spheres[i].vz);
    }
    myfile << "momentum: (" << momentumx << "," << momentumy << "," << momentumz << ") " << endl;
}

float calculateTime(Sphere sp1, Sphere sp2)
{
    float a, b, c, time;
    // float v2Mv1 = sqrt(pow((sp2.vx - sp1.vx), 2) + pow((sp2.vy - sp1.vy), 2) + pow((sp2.vz - sp1.vz), 2));
    // float p2Mp1 = sqrt(pow((sp2.posx - sp1.posx), 2) + pow((sp2.posy - sp1.posy), 2) + pow((sp2.posz - sp1.posz), 2));

    // a = pow(v2Mv1, 2);
    // b = 2 * v2Mv1 * p2Mp1;
    //  if(sp2.name=="universe"){
    //     c = pow(p2Mp1, 2) - pow((sp2.radius - sp1.radius), 2);
    // }else{
    //     c = pow(p2Mp1, 2) - pow((sp2.radius + sp1.radius), 2);
    // }

    float v2Mv1 = pow((sp2.vx - sp1.vx), 2) + pow((sp2.vy - sp1.vy), 2) + pow((sp2.vz - sp1.vz), 2);
    float p2Mp1 = pow((sp2.posx - sp1.posx), 2) + pow((sp2.posy - sp1.posy), 2) + pow((sp2.posz - sp1.posz), 2);
    b = 2 * ((sp2.vx - sp1.vx) * (sp2.posx - sp1.posx) + (sp2.vy - sp1.vy) * (sp2.posy - sp1.posy) + (sp2.vz - sp1.vz) * (sp2.posz - sp1.posz));
    a = v2Mv1;
    if (sp2.name == "universe")
    {
        c = p2Mp1 - pow((sp2.radius - sp1.radius), 2);
    }
    else
    {
        c = p2Mp1 - pow((sp2.radius - sp1.radius), 2);
    }

    float disk = sqrt(pow(b, 2) - 4 * a * c);
    time = (-b + disk) / (2 * a);

    if (time < 0)
    {
        time = (-b - disk) / (2 * a);
    }
    // cout << time<< endl;
    return time;
}

vector<Sphere> setNewPositions(float time, vector<Sphere> spheres)
{
    for (int i = 0; i < spheres.size(); ++i)
    {
        spheres[i].posx = spheres[i].posx + (time * spheres[i].vx);
        spheres[i].posy = spheres[i].posy + (time * spheres[i].vy);
        spheres[i].posz = spheres[i].posz + (time * spheres[i].vz);
    }
    return spheres;
}

vector<Sphere> setNewVelocity(float time, Sphere sp1, Sphere sp2)
{
    Sphere initial_sp1 = sp1;
    Sphere initial_sp2 = sp2;

    float vxSubtract = initial_sp1.vx - initial_sp2.vx;
    float vySubtract = initial_sp1.vy - initial_sp2.vy;
    float vzSubtract = initial_sp1.vz - initial_sp2.vz;

    float posxSubtract = initial_sp1.posx - initial_sp2.posx;
    float posySubtract = initial_sp1.posy - initial_sp2.posy;
    float poszSubtract = initial_sp1.posz - initial_sp2.posz;

    float DPvp = vxSubtract * posxSubtract + vySubtract * posySubtract + vzSubtract * poszSubtract;
    float DPpp = posxSubtract * posxSubtract + posySubtract * posySubtract + poszSubtract * poszSubtract;
    float DPvpDevideDPpp = DPvp / DPpp;

    posxSubtract = DPvpDevideDPpp * posxSubtract;
    posySubtract = DPvpDevideDPpp * posySubtract;
    poszSubtract = DPvpDevideDPpp * poszSubtract;

    if (sp2.name == "universe")
    {
        posxSubtract = 2 * posxSubtract;
        posySubtract = 2 * posySubtract;
        poszSubtract = 2 * poszSubtract;
    }
    else
    {
        float masses = 2 * initial_sp2.mass / (initial_sp1.mass + initial_sp2.mass);
        posxSubtract = masses * posxSubtract;
        posySubtract = masses * posySubtract;
        poszSubtract = masses * poszSubtract;
    }

    float vxf = initial_sp1.vx - posxSubtract;
    float vyf = initial_sp1.vy - posySubtract;
    float vzf = initial_sp1.vz - poszSubtract;

    sp1.vx = vxf;
    sp1.vy = vyf;
    sp1.vz = vzf;

    // sp1.printSphere();

    if (sp2.name != "universe")
    {
        vxSubtract = initial_sp2.vx - initial_sp1.vx;
        vySubtract = initial_sp2.vy - initial_sp1.vy;
        vzSubtract = initial_sp2.vz - initial_sp1.vz;

        posxSubtract = initial_sp2.posx - initial_sp1.posx;
        posySubtract = initial_sp2.posy - initial_sp1.posy;
        poszSubtract = initial_sp2.posz - initial_sp1.posz;

        DPvp = vxSubtract * posxSubtract + vySubtract * posySubtract + vzSubtract * poszSubtract;
        DPpp = posxSubtract * posxSubtract + posySubtract * posySubtract + poszSubtract * poszSubtract;
        DPvpDevideDPpp = DPvp / DPpp;

        posxSubtract = DPvpDevideDPpp * posxSubtract;
        posySubtract = DPvpDevideDPpp * posySubtract;
        poszSubtract = DPvpDevideDPpp * poszSubtract;

        float masses = 2 * initial_sp1.mass / (initial_sp2.mass + initial_sp1.mass);

        posxSubtract = masses * posxSubtract;
        posySubtract = masses * posySubtract;
        poszSubtract = masses * poszSubtract;

        // posxSubtract = 2 * posxSubtract;
        // posySubtract = 2 * posySubtract;
        // poszSubtract = 2 * poszSubtract;

        vxf = initial_sp2.vx - posxSubtract;
        vyf = initial_sp2.vy - posySubtract;
        vzf = initial_sp2.vz - poszSubtract;

        sp2.vx = vxf;
        sp2.vy = vyf;
        sp2.vz = vzf;

        // sp2.printSphere();
    }
    // else
    // {
    //     sp2.printSphere();
    // }

    vector<Sphere> final_spheres = {sp1, sp2};
    // final_spheres[0] = sp1;
    // final_spheres[1] = sp2;

    return final_spheres;
}

void sphereValues(vector<Sphere> spheres, Sphere universe, int collisions, string filename)
{
   
    for (int i = 0; i < spheres.size(); ++i)
    {
        myfile << spheres[i].name << " m=" << spheres[i].mass << " R=" << spheres[i].radius << " p=(" << spheres[i].posx << "," << spheres[i].posy << "," << spheres[i].posz << ") v=(" << spheres[i].vx << "," << spheres[i].vy << "," << spheres[i].vz << ") bounces=" << spheres[i].bounces << endl;
    }
    myfile << "energy: " << calculateEnergy(spheres) << endl;
    calculateMomentum(spheres);
    myfile << endl;
}

void startValues(vector<Sphere> spheres, Sphere universe, int collisions, string filename)
{
    myfile << "Here are the initial conditions.\n";
    myfile << "universe radius " << universe.radius << endl;
    myfile << "max collisions " << collisions << endl;
    sphereValues(spheres, universe, collisions, filename);
    myfile << "Here are the events.\n\n";
}

bool endOfcollisions(vector<Sphere> spheres, int collisions)
{
    if (spheres.size() <= 0)
    {
        return false;
    }
    return true;
}

void checkReflection(vector<Sphere> spheres, Sphere universe, int collisions, string filename)
{
    startValues(spheres, universe, collisions, filename);
    float total_time = 0;
    while (endOfcollisions(spheres, collisions))
    {

        float time = calculateTime(spheres[0], universe);
        Sphere csp1 = spheres[0];
        Sphere csp2 = universe;

        for (int i = 0; i < spheres.size(); ++i)
        {
            for (int j = 1; j < spheres.size(); ++j)
            {
                if (i != j)
                {
                    float ref = calculateTime(spheres[i], spheres[j]);
                    if (ref > 0 && ref < time)
                    {
                        time = ref;
                        csp1 = spheres[i];
                        csp2 = spheres[j];
                    }
                }
            }
            float ref = calculateTime(spheres[i], universe);
            if (ref > 0 && ref < time)
            {
                time = ref;
                csp1 = spheres[i];
                csp2 = universe;
            }
        }
        total_time += time;

        if (csp1.name != "universe")
        {
            csp1.bounces++;
        }
        if (csp2.name != "universe")
        {
            csp2.bounces++;
        }
        vector<Sphere> final_velocity_of_spheres;
        final_velocity_of_spheres = setNewVelocity(time, csp1, csp2);

        vector<Sphere> final_positions_of_spheres;
        final_positions_of_spheres = setNewPositions(time, spheres);

        for (int i = 0; i < spheres.size(); i++)
        {
            for (int j = 0; j < spheres.size(); j++)
            {
                if (spheres[i].name == final_velocity_of_spheres[j].name)
                {
                    spheres[i].vx = final_velocity_of_spheres[j].vx;
                    spheres[i].vy = final_velocity_of_spheres[j].vy;
                    spheres[i].vz = final_velocity_of_spheres[j].vz;
                    spheres[i].bounces = final_velocity_of_spheres[j].bounces;
                }
                if (spheres[i].name == final_positions_of_spheres[j].name)
                {
                    spheres[i].posx = final_positions_of_spheres[j].posx;
                    spheres[i].posy = final_positions_of_spheres[j].posy;
                    spheres[i].posz = final_positions_of_spheres[j].posz;
                }
            }
        }
        if(isnan(time)){
            myfile.close();
            break;
        }
        myfile << "\ntime of event: " << total_time << endl;
        if (csp2.name == "universe")
        {
            myfile << "reflecting " << csp1.name << endl;
        }
        else
        {
            myfile << "colliding " << csp1.name << " " << csp2.name << endl;
        }
        sphereValues(spheres, universe, collisions, filename);
        vector<int> indexes;
        for (int i = 0; i < spheres.size(); i++)
        {
            if (spheres[i].bounces >= collisions)
            {
                myfile << "disappear " << spheres[i].name << "\n";
                spheres[i] = spheres.back();
                spheres.pop_back();
            }
        }
        for (int i = 0; i < spheres.size(); i++)
        {
            if (spheres[i].bounces >= collisions)
            {
                myfile << "\ndisappear " << spheres[i].name << "\n";
                spheres[i] = spheres.back();
                spheres.pop_back();
            }
        }
    }

}

void start()
{
    cout << "Please enter the mass, radius, x/y/z posicollisionstion, x/y/z velocity \nand name of each sphere \nWhen complete, use EOF / Ctrl-D to stop entering" << endl;
}

int main(int argc, char *argv[])
{
    float universe_radius;
    int collisions;
    string filename;
    vector<Sphere> spheres;
    if (argc == 1)
    {
        printf("\nPlease enter mass and radius values");
        return 0;
    }
    else if (argc == 2)
    {
        printf("\nPlease enter radius value");
        return 0;
    }
    else if (argc == 4)
    {
        filename = argv[3];
        spheres = readInputFile(filename);
    }

    universe_radius = atof(argv[1]);
    collisions = atoi(argv[2]);


    string a = filename;
    a = a.erase(a.length()-4);
    cout << a<< endl;

    std::stringstream sstream;
    sstream << a << "_" << universe_radius << "_" << collisions << "_JBC.txt";
    filename = sstream.str();
    myfile.open(filename);

    

    Sphere universe(0, universe_radius, 0, 0, 0, 0, 0, 0, "universe");
    checkReflection(spheres, universe, collisions, filename);


    // Sphere sphere1;
    // Sphere sphere2(12, 3, 0,0,0, 0,0,1, "name");
    // sphere1.printSphere();
    // sphere2.printSphere();

    // Sphere sphere2(1,2,3,4,5,6,7,8, 'mtsphere');
    // sphere1.printSphere()
    //
    // writeOutputFile("Hello world");

    // universe_radius = 100;
    // collisions = 2;
    // Sphere universe(0, universe_radius, 0, 0, 0, 0, 0, 0, "universe");
    // Sphere s1(1, 10, -12, -12, -12, -3, -3, -3, "pingpong");
    // Sphere s2(100, 1, 2.2, 2.2, 2.2, 2, 2, 2, "marble");
    // Sphere s1(1, 10, -51.9615,-51.9615,-51.9615, 3, 3, 3, "pingpong");
    // Sphere s2(100, 1, 28.841,28.841,28.841, 2,2,2, "marble");
    // Sphere s1(1, 10, -9.48653, -9.48653, -9.48653, 3, 3, 3, "pingpong");
    // Sphere s2(100, 1, 57.1577, 57.1577, 57.1577, -2, -2, -2, "marble");

    // vector<Sphere> spheres = {s1, s2};
    // checkReflection(spheres, universe, collisions);

    // universe_radius = 120.0;
    // collisions = 2;
    // Sphere universe(0, universe_radius, 0, 0, 0, 0, 0, 0, "universe");
    // Sphere s1(20, 1, 0, 0, 0, 0, 0, 1, "one  ");
    // Sphere s2(2, 5, 0, 1, 100, 0, 0, 0, "two  ");
    // Sphere s3(3, 1, 2, -1, -2, 0, 0, 0, "three");

    // vector<Sphere> spheres = {s1, s2, s3};
    // checkReflection(spheres, universe, collisions);

    // calculateTime(s1, universe);
    // calculateTime(s2, universe);
    // calculateTime(s2, s1);
    // calculateTime(s1, s2);

    // universe_radius = 2000.0;
    // collisions = 2;
    // Sphere universe(0, universe_radius, 0, 0, 0, 0, 0, 0, "universe");
    // Sphere s1(1, 5, 10,1000,4, 0,-10,0, "pool");
    // Sphere s2(2, 6, 10,500 ,4, 0,0  ,0, "one ");
    // Sphere s3(3, 7, 10,400 ,4, 0,0  ,0, "two ");
    // Sphere s4(4, 8, 10,300 ,4, 0,0  ,0, "thr ");
    // Sphere s5(5, 9, 10,200 ,5, 0,0  ,0, "fou ");spheres
    // Sphere s6(6, 1, 10,100 ,6, 0,0  ,0, "fiv ");
    // Sphere s7(7, 2, 10,0   ,7, 0,0  ,0, "six ");
    // Sphere s8(8, 3, 10,-100,8, 0,0  ,0, "sev ");

    // vector<Sphere> spheres = {s1, s2, s3, s4, s5, s6, s7, s8};
    // checkReflection(spheres, universe, collisions);

    // float energy = calculateEnergy(s3, s4);
    // cout << energy << endl;
    // float *momentum = calculateMomentum(s3, s4);
    // cout << momentum[0]<< " " << momentum[1]<< " " << momentum[2]<< " " << endl;
}
