import random
from tabulate import tabulate


class Schedule:

    n_days = 5
    n_lessons = 3
    total_lessons = n_days * n_lessons
    n_teachers = 4
    n_groups = 3
    n_rooms = 5
    n_subjects = 6
    n_s_per_t = 5 
    n_s_per_g = 3 

    def __init__(self, g_names, s_names, t_names, week_days):
        self.g_names = g_names
        self.s_names = s_names
        self.t_names = t_names
        self.week_days = week_days
        self.rooms = [f"R{i}" for i in range(1, self.n_rooms + 1)]
        self.groups = g_names[:self.n_groups]
        self.subjects = s_names[:self.n_subjects]
        self.teachers = t_names[:self.n_teachers]

        self.rooms_dests = ["lec"] * self.n_rooms
        for i in range(0, self.n_rooms, 2):
            self.rooms_dests[i] = "lab"

        # subjects teacher cat teach
        self.teacher_specs = [set([(si ** 2 + ti ** 2) % self.n_subjects for si in range(self.n_s_per_t)])
                              for ti in range(self.n_teachers)]

        # subjects group is being taught
        self.learning_plan = [set([(si ** 2 + gi) % self.n_subjects for si in range(self.n_s_per_g)])
                              for gi in range(self.n_groups)]

        # room per lesson per group
        self.rpl = [[None] * self.n_groups for _ in range(self.total_lessons)]

        # subject per lesson per group
        self.spl = [[None] * self.n_groups for _ in range(self.total_lessons)]

        # teacher per lesson per group
        self.tpl = [[None] * self.n_groups for _ in range(self.total_lessons)]

        self.cnt = 0

    def is_complete(self):
        return all([not any(lg is None for lg in l) for l in self.rpl])

    def check_constraints(self):
        self.cnt += 1
        
        # classes are complete if only they have both labs and lectures
        if self.is_complete():
            for g in range(self.n_groups):
                class_types_per_subjects = {s: set() for s in self.learning_plan[g]}
                for l in range(self.total_lessons):
                    class_types_per_subjects[self.spl[l][g]].add(self.rooms_dests[self.rpl[l][g]])
                if any([len(s) != 2 for s in class_types_per_subjects.values()]):
                    return False

        # each teacher can teach only 1 group at a time
        for tpg in self.tpl:
            for i in range(self.n_groups - 1):
                if tpg[i] is not None and tpg[i] in tpg[i + 1:]:
                    return False

        # each room can be used only by 1 group at a time
        for rpg in self.rpl:
            for i in range(self.n_groups - 1):
                if rpg[i] is not None and rpg[i] in rpg[i + 1:]:
                    return False

        return True

    def setter(self, l, g, t, r, s):
        self.tpl[l][g] = t
        self.rpl[l][g] = r
        self.spl[l][g] = s
        
    def clear(self):
        self.rpl = [[None] * self.n_groups for _ in range(self.total_lessons)]
        self.spl = [[None] * self.n_groups for _ in range(self.total_lessons)]
        self.tpl = [[None] * self.n_groups for _ in range(self.total_lessons)]

    def select_unassigned_var(self):
        for l in range(self.total_lessons):
            for g in range(self.n_groups):
                if self.tpl[l][g] is None:
                    return l, g

        # for l in random.sample(range(self.total_lessons), self.total_lessons):
        #     for g in random.sample(range(self.n_groups), self.n_groups):
        #         if self.tpl[l][g] is None:
        #             return l, g

    def degree_heuristic(self):
        none_list = []
        for l in range(self.total_lessons):
            none_list.append(sum([self.tpl[l][g] is None for g in range(self.n_groups)]))
        l = none_list.index(max(none_list))
        for g in range(self.n_groups):
            if self.tpl[l][g] is None:
                return l, g

    def mrv(self):
        for d in range(self.n_days):
            for l in range(self.n_lessons):
                l = d * self.n_lessons + l
                for g in range(self.n_groups):
                    if self.tpl[l][g] is None:
                        return l, g

    def order_domain_vals(self, g):
        for t in random.sample(range(self.n_teachers), self.n_teachers):
            available_subjects = list(self.learning_plan[g].intersection(self.teacher_specs[t]))
            for r in random.sample(range(self.n_rooms), self.n_rooms):
                for s in random.sample(available_subjects, len(available_subjects)):
                    yield t, r, s

    def lcv(self, g):
        teacher_scores = []
        for t in range(self.n_teachers):
            teacher_scores.append([0, t])
            for gi in range(self.n_groups):
                if gi != g:
                    teacher_scores[-1][0] += len(self.teacher_specs[t].intersection(self.learning_plan[gi]))
        # print(teacher_scores)
        for _, t in sorted(teacher_scores, key=lambda sc: sc[0]):
            available_subjects = list(self.learning_plan[g].intersection(self.teacher_specs[t]))
            for r in random.sample(range(self.n_rooms), self.n_rooms):
                for s in random.sample(available_subjects, len(available_subjects)):
                    yield t, r, s

    def forward_check(self, l, g):
        for t in random.sample(range(self.n_teachers), self.n_teachers):
            if t not in self.tpl[l]:
                available_subjects = list(self.learning_plan[g].intersection(self.teacher_specs[t]))
                for r in random.sample(range(self.n_rooms), self.n_rooms):
                    if r not in self.rpl[l]:
                        for s in random.sample(available_subjects, len(available_subjects)):
                            yield t, r, s

    def ac3(self):
        queue = []
        for l in range(self.total_lessons):
            for g1 in range(self.n_groups):
                for g2 in range(g1 + 1, self.n_groups):
                    queue.append((l, g1, g2))

        while queue:
            l, g1, g2 = queue.pop(0)
            if self.remove_inconsistent_val(l, g1, g2):
                for g in range(self.n_groups):
                    if g != g1:
                        queue.append((l, g1, g))

    def remove_inconsistent_val(self, l, g1, g2):
        good_subjects = set()
        bad_subjects = set()
        for t, r, s in self.order_domain_vals(g1):
            self.setter(l, g1, t, r, s)
            if self.check_constraints():
                constraints_checker = []

                for t, r, s in self.order_domain_vals(g2):
                    self.setter(l, g2, t, r, s)
                    constraints_checker.append(self.check_constraints())
                    self.setter(l, g2, None, None, None)

                if not any(constraints_checker):
                    bad_subjects.add(s)
                else:
                    good_subjects.add(s)
            self.setter(l, g1, None, None, None)
        subjects_to_remove = bad_subjects.difference(good_subjects)
        for s in subjects_to_remove:
            self.learning_plan[g1].remove(s)
        return len(subjects_to_remove) > 0

    def backtracking(self):
        if self.cnt > 1e3:
            self.clear()
            self.cnt = 0
        
        var = self.degree_heuristic() # self.mrv()  #  self.select_unassigned_var() # 

        if var is None:
            return True

        l, g = var

        for t, r, s in self.forward_check(l, g): # self.lcv(g): #self.forward_check(l, g): self.order_domain_vals(g):  # 
            self.setter(l, g, t, r, s)

            if self.check_constraints():
                res = self.backtracking()
                if res:
                    return True
            self.setter(l, g, None, None, None)

        return False

    def print(self):
        table = dict(indices=["Day", "Group"] + list(range(1, self.n_lessons + 1)))
        for d in range(self.n_days):
            table[(d, 0)] = [self.week_days[d]]
            for g in range(1, self.n_groups):
                table[(d, g)] = [""]
            for g in range(self.n_groups):
                table[(d, g)].append(self.g_names[g])
                for l in range(self.n_lessons):
                    l = d * self.n_lessons + l
                    lesson = (f"{self.subjects[self.spl[l][g]]}\n"
                              f"({self.rooms_dests[self.rpl[l][g]]})\n"
                              f"Room: {self.rooms[self.rpl[l][g]]}\n"
                              f"Prof.: {self.teachers[self.tpl[l][g]]}")
                    table[(d, g)].append(lesson)
            table[(d, -1)] = [""] * (2 + self.n_lessons)
        return tabulate(table, tablefmt="html")



