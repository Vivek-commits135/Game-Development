import pygame
import random
pygame.init()
pygame.key.set_repeat(10, 300)
window=pygame.display.set_mode((1000, 500))
pygame.display.set_caption("Flappy Bird")
pygame.display.set_icon(pygame.image.load("bird_small.png"))
images={"bird": pygame.image.load('bird.png')}
accel=200
speed=0
height=250
exited=0
tick=0
score=0
time=pygame.time.Clock()
font = pygame.font.SysFont(None, 72)
restart_text = font.render("Restart", True, (0, 0, 0))
exit_text = font.render("Exit", True, (0, 0, 0))
restart_rect = restart_text.get_rect(center=(500, 160))
exit_rect = exit_text.get_rect(center=(500, 340))
def dist(t1, t2) :
    return (((t1[0]-t2[0])**2) + ((t1[1]-t2[1])**2))**(1/2)
def bird() :
    window.blit(images["bird"], (333-50, height-50))
class Pipes :
    list=[]
    hitboxes=[]
    speed=200
    def append(self) :
        rand=random.randint(150, 450)
        self.list.append([rand, 1000])
        self.hitboxes.append([pygame.Rect(1000-60, rand-20, 80, 500-rand+20), pygame.Rect(1000-60, 0, 80, rand-100+20)])
    def update(self) :
        for tuple in self.list :
            index=self.list.index(tuple)
            if tuple[1]>0 :
                tuple[1]-=self.speed*dt
                self.hitboxes[index]=[pygame.Rect(tuple[1]-60, tuple[0]-20, 80, 500-tuple[0]+20), pygame.Rect(tuple[1]-60, 0, 80, tuple[0]-100+20)]
            else :
                self.list.pop(index)
                self.hitboxes.pop(index)
    def void(self) :
        self.list=[]
        self.hitboxes=[]
    def show(self) :
        for tuple in self.list :
            window.fill((0, 255, 0), (tuple[1]-40, tuple[0], 40, 500-tuple[0]))
            window.fill((0, 255, 0), (tuple[1]-40, 0, 40, tuple[0]-100))
    def collision(self, bird) :
        for hitbox in self.hitboxes :
            if hitbox[0].collidepoint(bird) or hitbox[1].collidepoint(bird) :
                return True
        return False
    def passed(self) :
        for tuple in self.list :
            if tuple[1]>=333-50 and tuple[1]<=333-50+(self.speed*dt) :
                return True
        return False
pipes=Pipes()
while not exited :
    menu=1
    while height>500 or height<0 or pipes.collision((333, height)) :
        window.fill((0, 0, 0))
        window.fill((0, 255, 0), restart_rect)
        window.fill((255, 0, 0), exit_rect)
        window.blit(restart_text, restart_rect)
        window.blit(exit_text, exit_rect)
        pygame.display.update()
        for event in pygame.event.get() :
            if event.type == pygame.QUIT :
                exited=1
                menu=0
            if event.type == pygame.MOUSEBUTTONDOWN :
                if event.button == 1 and restart_rect.collidepoint(pygame.mouse.get_pos()) :
                    pipes.void()
                    speed=0
                    height=250
                    tick=0
                    score=0
                    break
                elif event.button == 1 and exit_rect.collidepoint(pygame.mouse.get_pos()) :
                    exited=1
                    menu=0
        if not menu :
            break
        time.tick(60)
    dt=time.tick(60)/1000
    window.fill((0, 0, 0))
    for event in pygame.event.get() :
        if event.type == pygame.QUIT :
            exited=1
            break
        if event.type == pygame.KEYDOWN and event.key == pygame.K_UP :
            speed=-7500*dt
    if speed < 150 :
        speed+=accel*dt
    height+=speed*dt
    bird()
    if tick%120 == 0 :
        pipes.append()
    pipes.update()
    pipes.show()
    if pipes.passed() :
        score+=1
    score_text=font.render("Score: "+str(score), True, (255,255,255))
    window.blit(score_text, (10, 10, 100, 50))
    pygame.display.update()
    tick+=1
