import random

# Step 1: Simulate venue data
venues = {
    "Venue A": {"price": 100.10, "liquidity": 2000, "latency": 10, "rebate": 0.01},
    "Venue B": {"price": 100.20, "liquidity": 1500, "latency": 5, "rebate": 0.02},
    "Venue C": {"price": 100.05, "liquidity": 2500, "latency": 15, "rebate": 0.005},
}

# Step 2: Normalize data
def normalize(values):
    min_val = min(values)
    max_val = max(values)
    return [(val - min_val) / (max_val - min_val) for val in values]

prices = normalize([v["price"] for v in venues.values()])
liquidity = normalize([v["liquidity"] for v in venues.values()])
latency = normalize([v["latency"] for v in venues.values()])
rebates = normalize([v["rebate"] for v in venues.values()])

# Step 3: Define weights
weights = {"price": 0.5, "liquidity": 0.3, "latency": -0.15, "rebate": 0.05}

# Step 4: Calculate venue scores
scores = {}
for i, venue in enumerate(venues.keys()):
    scores[venue] = (
        weights["price"] * prices[i]
        + weights["liquidity"] * liquidity[i]
        + weights["latency"] * latency[i]
        + weights["rebate"] * rebates[i]
    )

# Step 5: Order splitting
order_size = 1000
total_score = sum(scores.values())
order_distribution = {venue: (score / total_score) * order_size for venue, score in scores.items()}

# Output results
print("Venue Scores:", scores)
print("Order Distribution:", order_distribution)
