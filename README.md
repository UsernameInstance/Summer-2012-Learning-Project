# Summer-2012-Learning-Project
see https://www.youtube.com/watch?v=VpUhnLqkHA8

A small program I put together using some of my free time last summer (2012) as a learning project. I wanted to learn the basics of SDL, neural networks, and 2D physics simulation. The result was a small, I suppose you could call it, ecosystem simulation. 

The animals have no legs and have crude billiard ball physics. Movement is controlled via forward or backward force output and clockwise or counterclockwise torque. All movements have a corresponding energy cost. Animals also have a constant energy loss dependent on their mass, where mass is proportional to the animals visual size. 

Horns can steal life from other animals. The amount of life stolen is proportional to the linear momentum of the attacking animal when it hits. Spinning will also cause a small amount of energy steal. 

Passing or sitting over green vegetation squares allows animals to slowly sap energy from them. 

The animals are asexual. A heart represents reproduction. When the animal reproduces it chooses how much energy to give to its offspring. So it's possible offspring may die immediately. The offspring has a mutated set of its parents' attributes. The brain retains its learning save for any random mutations that occur. 

The brain is a crude 3 layer feedforward neural network. The learning is also crude. I tossed in some code that updates the neural nets weights depending on energy gain/lose trends of a small saved collection of previous input-output states each separated by some small time interval and updated after some small time interval. 

Each animal has a single eye that can detect vegetation or other animals. They eye has 4 neurons attached to it. The neurons measure the amount of red, green, and blue. As well as distance from the animal. 

Each animal also has a neuron that detects the current tile it's center is over (dirt, vegetation, or water) as well as touch neurons that activate on collision and detect angle of collision and force. 

They also have neurons that detect self movement properties, health, and rate of health change per second. 

Green squares are healthy vegetation. They will periodically reproduce. Blue squares are water and increase rate of energy gain for adjacent vegetation. Vegetation is on a forced death timer to force it to spread across the map over time. Vegetation turns brown as it loses energy.

Results of the sim are somewhat limited. Behavior seems limited to linear 'ramming' animals and somewhat stationary 'spinners'. Larger animals are usually spinners while ramming animals are usually smaller. I'd guess this is because larger animals can get more life from vegetation as they can cover a lot more at once than a small animal. On the other hand smaller animals usually have better mobility.
