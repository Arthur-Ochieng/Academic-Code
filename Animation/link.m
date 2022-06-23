world = vrworld('assign.WRL');
open(world);

fig = view(world, '-internal');
vrdrawnow;

Lotus = vrnode(world, 'Lotus');
Man = vrnode(world, 'Man');
Woman = vrnode(world, 'Woman');
Boy = vrnode(world, 'Boy');
Girl = vrnode(world, 'Girl');

%{
Camera.position = [Lotus.translation(1), 0, 0];

while Lotus.translation(1)<7.5
    LotusX = Lotus.translation(1)+1;
    Lotus.translation = [LotusX,Lotus.translation(2),Lotus.translation(3)];
        vrdrawnow;
        pause(0.1);
end
%}

%Initial car position
z1 = 0.0000:7.5;
x1 = 16.9220 + zeros(size(z1));
y1 = 0.0030 + zeros(size(z1));

%Position before the first turn
x2 = 16.9220:-60.0780;
z2 = 7.5 + zeros(size(x2));
y2 = 0.0030 + zeros(size(z2));

%reaching school
z3 = 7.5:-20.7;
x3 = -60.0780 + zeros(size(x2));
y3 = 0.0030 + zeros(size(x2));

%reaching the workplace
x4 =-60.0780 + zeros(size(x2));
y4 = 0.0030 + zeros(size(x2));
z4 = -20.7 + zeros(size(x2));


for i=1:length(x1)
    Lotus.translation = [x1(i) y1(i) z1(i)];
    vrdrawnow;
    pause(0.1);
end

Lotus.rotation = [0, 1, 0, -1.5708];
vrdrawnow;


for i=1:length(x2)
    Lotus.translation = [x2(i) y2(i) z2(i)];
    vrdrawnow;
    pause(0.05);
end

for i=1:length(z3)
    Lotus.translation = [x3(i) y3(i) z3(i)];
    vrdrawnow;
    pause(0.05);
end

Lotus.rotation = [0, -1, 0, deg2rad(180)];
vrdrawnow;

for i=1:length(x4)
    Lotus.translation = [x4(i) y4(i) z4(i)];
    vrdrawnow;
    pause(0.05);
end


reload(world);
vrdrawnow;
