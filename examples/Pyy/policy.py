import torch
import torch.nn.functional as F
import numpy as np

class policy(torch.nn.Module):
    """
    implements a policy model
    """
    def __init__(self):
        super(policy, self).__init__()
        self.affine1 = torch.nn.Linear(1, 128)
        self.linear = torch.nn.Linear(128, 1)
    def forward(self, x):
        """
        forward 
        """
        x = torch.from_numpy(np.array([x])).float() # convert obs to Torch tensor
        x = F.relu(self.affine1(x))
        x = F.sigmoid(self.linear(x))
        return x
model = policy()
torch.save(model.state_dict(), 'policy.pt')
