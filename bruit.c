/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bruit.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alesecq <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/10 17:04:25 by alesecq           #+#    #+#             */
/*   Updated: 2016/12/10 17:54:39 by alesecq          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define B 0x100
#define BM 0xFF
#define N 0x1000
#define NP 12
#define NM 0xFFF
#define S_CURVE(t) (t * t * (3.0 - 2.0 * t))
#define LERP(t, a, b) (a + t * (b - a))
#define AT3(rx,ry,rz) ( rx * n.q.x + ry * n.q.y + rz * n.q.z )
#define AT3_POINT(rx,ry,rz) ( rx * n->q.x + ry * n->q.y + rz * n->q.z )

typedef	struct	s_perlin
{
	int bx0;
	int bx1;
	int by0;
	int by1;
	int bz0;
	int bz1;
	int b00;
	int b10;
	int	b01;
	int b11;
	float rx0;
	float rx1;
	float ry0;
	float ry1;
	float rz0;
	float rz1;
	float sx;
	float sy;
	float a;
	float3 q;
	float b;
	float c;
	float d;
	float t;
	float u;
	float v;
}				t_perlin;

static int g[B + B + 2];
static float3 g_g3[B + B + 2];
static int g_start = 1;

void	setup_x(t_perlin *p, float3 vec)
{
	p->t = vec.x + N;
	p->bx0 = ((int)p->t) & BM;
	p->bx1 = (p->bx0 + 1) & BM;
	p->rx0 = p->t - (int)p->t;
	p->rx1 = p->rx0 - 1.0;
}

void setup_y(t_perlin *p, float3 vec)
{
	p->t = vec.y + N;
	p->by0 = ((int)p->t) & BM;
	p->by1 = (p->by0 + 1) & BM;
	p->ry0 = p->t - (int)p->t;
	p->ry1 = p->ry0 - 1.0;
}

void setup_z(t_perlin *p, float3 vec)
{
	p->t = vec.z + N;
	p->bz0 = ((int)p->t) & BM;
	p->bz1 = (p->bz0 + 1) & BM;
	p->rz0 = p->t - (int)p->t;
	p->rz1 = p->rz0 - 1.0;
}

void init_perlin(void)
{
	int i;
	int j;
	int k;

	i = 0;
	while(i < B)
	{
		g[i] = i;
		g_g3[i].x = (float)((random() % (B + B)) - B) / B;
		g_g3[i].y = (float)((random() % (B + B)) - B) / B;
		g_g3[i].z = (float)((random() % (B + B)) - B) / B;
		soft_normalize(&g_g3[i]);
		i++;
	}
	while (--i)
	{
		k = g[i];
		g[i] = g[j = random() % B];
		g[j] = k;
	}
	i = 0;
	while (i < B + 2)
	{
		g[B + i] = g[i];
		g_g3[B + i].x = g_g3[i].x;
		g_g3[B + i].y = g_g3[i].y;
		g_g3[B + i].z = g_g3[i].z;
		i++;
	}
}

float noise(float3 vec)
{
	t_perlin p;
	int i;
	int j;

	if (g_start)
	{
		g_start = 0;
		init_perlin();
	}
	setup_x(&p, vec);
	setup_y(&p, vec);
	setup_z(&p, vec);
	i = g[p.bx0];
	j = g[p.bx1];
	p.b00 = g[i + p.by0];
	p.b10 = g[j + p.by0];
	p.b01 = g[i + p.by1];
	p.b11 = g[j + p.by1];
	p.t = S_CURVE(p.rx0);
	p.sy = S_CURVE(p.ry0);
	p.sz = S_CURVE(p.rz0);
	p.q = g_g3[p.b00 + p.bz0];
	p.u = AT3_POINT(p.rx0, p.ry0, p.rz0);
	p.q = g_g3[p.b10 + p.bz0];
	p.v = AT3_POINT(p.rx1, p.ry0; p.rz0);
	p.a = LERP(p.t, p.u, p.v);
	p.q = g_g3[p.b01 + p.bz0];
	p.u = AT3_POINT(p.rx0, p.ry1, p.rz0);
	p.q = g_g3[p.b11 + b.z0];
	p.v = AT3_POINT(p.rx1, p.ry1, p.rz0);
	p.b = LERP(p.t, p.u, p.v);
	p.c = LERP(p.sy, p.a, p.b);
	p.q = g_g3[p.b00 + p.bz1 ];
   	p.u = AT3_POINT(p.rx0, p.ry0, p.rz1);
	p.q = g_g3[p.b10 + p.bz1 ];
   	p.v = AT3_POINT(p.rx1, p.ry0, p.rz1);
	p.a = LERP(p.t, p.u, p.v);
	p.q = g_g3[p.b01 + p.bz1 ];
	p.u = AT3_POINT(p.rx0, p.ry1, p.rz1);
	p.q = g_g3[p.b11 + p.bz1 ];
   	p.v = AT3_POINT(p.rx1, p.ry1, p.rz1);
	p.b = LERP(p.t, p.u, p.v);
	p.d = LERP(p.sy, p.a, p.b);
	return (LERP(p.sz, p.c, p.d));
}
