//extern const int m=m; //number of lattice points 

void Create_temporal_FEM(int cell_1[251][251], double deformation_1[251][251],int cell_2[251][251], int direction_2[251][251], double deformation_2[251][251], int iter, int migration_dir[251][251],int& number_vessels_temp, int tip_x_temp[(251-1)*20], int tip_y_temp[(251-1)*20], double X_temp[251*251],double Y_temp[251*251], int previous_dir_temp [(251-1)*20]);
void Initialize_model(int cell_matrix[251][251], int vessel_x[(251-1)*20][251-1], int vessel_y[(251-1)*20][251-1], int force_dir[251][251], int nodes[(251-1)*(251-1)][4], int fibro_number, int endo_number, int& number_vessels_init, int tip_vessel_x [(251-1)*20], int tip_vessel_y [(251-1)*20], int previous_dir [(251-1)*20], int length [(251-1)*20], double X[251*251], double Y[251*251]);
void Read_displacements(int cells[251][251], int dip_dir[251][251], double deformation[251][251], int t,int model_type, int& number_vessels_displ, int tip_x_displ[(251-1)*20], int tip_y_displ[(251-1)*20],  double X_read[251*251],double Y_read[251*251]);
void Update_model(int cell_matrix_1[251][251], int dipole_dir_1[251][251], double cell_def_1[251][251],int cell_matrix_2[251][251], int dipole_dir_2[251][251], double cell_def_2[251][251],int iter, int migration[251][251]);
void Proliferation_apoptosis(int cell_matrix[251][251], int dipole[251][251],int iter, int cell_number, int& number_vessels_prol, int tip_x_prol[(251-1)*20], int tip_y_prol[(251-1)*20], double X[251*251], double Y[251*251],int previous_dir_proli [(251-1)*20]);
void Write_batch_file(int iter, int model_type);

// Angiogenesis
void Blood_vessels(int cells_vessels[251][251],int vessels_x[(251-1)*20][251-1],int vessels_y[(251-1)*20][251-1], int& num_vessel, int tip_x[(251-1)*20], int tip_y[(251-1)*20], int previous_direction[(251-1)*20], int v_length[(251-1)*20], int iter);
int Direction_strains(int tip_x, int tip_y,double E11[251*251], double E22[251*251], double E12[251*251],float k[251*251]);
int Branching(int length);
float count_ec(int j, int i, int cells[251][251], int iter);
//
void Direction_growth_1(int cells_dir1[251][251],int j_dir1, int i_dir1, int& new_branch_j_dir1,int& new_branch_i_dir1,int& length_increase1,int& visual_i_dir1,int& visual_j_dir1);
void Direction_growth_2(int cells_dir2[251][251],int j_dir2, int i_dir2, int& new_branch_j_dir2,int& new_branch_i_dir2,int& length_increase2,int& visual_i_dir2,int& visual_j_dir2);
void Direction_growth_3(int cells_dir3[251][251],int j_dir3, int i_dir3, int& new_branch_j_dir3,int& new_branch_i_dir3,int& length_increase3,int& visual_i_dir3,int& visual_j_dir3);
void Direction_growth_4(int cells_dir4[251][251],int j_dir4, int i_dir4, int& new_branch_j_dir4,int& new_branch_i_dir4,int& length_increase4,int& visual_i_dir4,int& visual_j_dir4);
void Direction_growth_5(int cells_dir5[251][251],int j_dir5, int i_dir5, int& new_branch_j_dir5,int& new_branch_i_dir5,int& length_increase5,int& visual_i_dir5,int& visual_j_dir5);
void Direction_growth_6(int cells_dir6[251][251],int j_dir6, int i_dir6, int& new_branch_j_dir6,int& new_branch_i_dir6,int& length_increase6,int& visual_i_dir6,int& visual_j_dir6);
void Direction_growth_7(int cells_dir7[251][251],int j_dir7, int i_dir7, int& new_branch_j_dir7,int& new_branch_i_dir7,int& length_increase7,int& visual_i_dir7,int& visual_j_dir7);
void Direction_growth_8(int cells_dir8[251][251],int j_dir8, int i_dir8, int& new_branch_j_dir8,int& new_branch_i_dir8,int& length_increase8,int& visual_i_dir8,int& visual_j_dir8);
//
void Branch_dir1(int cells_branch1[251][251],int j_branch1, int i_branch1, int& previous_direction_branch1, int& new_j_branch1, int& new_i_branch1, int& length_branch1,int& visual_i_branch1,int& visual_j_branch1);
void Branch_dir2(int cells_branch2[251][251],int j_branch2, int i_branch2, int& previous_direction_branch2, int& new_j_branch2, int& new_i_branch2, int& length_branch2,int& visual_i_branch2,int& visual_j_branch2);
void Branch_dir3(int cells_branch3[251][251],int j_branch3, int i_branch3, int& previous_direction_branch3, int& new_j_branch3, int& new_i_branch3, int& length_branch3,int& visual_i_branch3,int& visual_j_branch3);
void Branch_dir4(int cells_branch4[251][251],int j_branch4, int i_branch4, int& previous_direction_branch4, int& new_j_branch4, int& new_i_branch4, int& length_branch4,int& visual_i_branch4,int& visual_j_branch4);
void Branch_dir5(int cells_branch5[251][251],int j_branch5, int i_branch5, int& previous_direction_branch5, int& new_j_branch5, int& new_i_branch5, int& length_branch5,int& visual_i_branch5,int& visual_j_branch5);
void Branch_dir6(int cells_branch6[251][251],int j_branch6, int i_branch6, int& previous_direction_branch6, int& new_j_branch6, int& new_i_branch6, int& length_branch6,int& visual_i_branch6,int& visual_j_branch6);
void Branch_dir7(int cells_branch7[251][251],int j_branch7, int i_branch7, int& previous_direction_branch7, int& new_j_branch7, int& new_i_branch7, int& length_branch7,int& visual_i_branch7,int& visual_j_branch7);
void Branch_dir8(int cells_branch8[251][251],int j_branch8, int i_branch8, int& previous_direction_branch8, int& new_j_branch8, int& new_i_branch8, int& length_branch8,int& visual_i_branch8,int& visual_j_branch8);
//
void Write_results(int cell_matrix[251][251], int dipole[251][251],int t_write);
void Write_results_blood_vessel(int cells_vessels[251][251],int time);
void Write_data_vessels(int time, int vessels_x[(251-1)*20][251-1],int vessels_y[(251-1)*20][251-1], int& num_vess);
void Read_cell_matrix(int cell_matrix[251][251], int dipole_matrix[251][251], int iteration);

     

    




