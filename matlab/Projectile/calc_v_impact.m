  function v_impact=calc_v_impact(g,v0,theta,t_flight,y0)
  % Calculate projectile impact velocity
  %
  % Usage: v_impact=calc_v_impact(g,v0,theta,t_flight,y0)
  %
  % where: g = gravitational acceleration (positive)
  %        v0 = initial velocity (magnitude)
  %        theta = launch angle (degrees)
  %        t_flight = time of flight
  %        x0 = launch distance from origin

  vx=v0*cos(pi*(theta/180));
  vy=-g*t_flight+v0*sin(pi*(theta/180));
  v_impact=sqrt(vx^2+vy^2);