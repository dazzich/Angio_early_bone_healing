// FIXED VARIABLES //
constexpr int N=251; //number of nodes from FEM on each edge (here the domain is a square)
constexpr float F_FIBRO = 0.000018;
constexpr float F_FIBROcos = 0.0000127;
constexpr float F_ENDO = 0.000010;
constexpr float F_ENDOcos = 0.00000707;
constexpr float STRAIN_1 = 0.05; // levels of the curve vessel orientation-strain
constexpr float STRAIN_2 = 0.10;
constexpr int DAY4=288;
// Periosteum & bone marrow 
/* 
   |    |       |    |
   | CL |_______| CR |
   
          Gap
         _______
   | CL |      | CR |
   |    |      |    |
*/
// constexpr int SAFE
constexpr int GAP_Down=101;
constexpr int GAP_Top=150;
constexpr int CL_left=50;
constexpr int CL_right=71;
constexpr int CR_left=180;
constexpr int CR_right=201;
constexpr int SAFE=80; // position until which bone marrow is not injured and host cells/vessels (we assume no vessels/cells for 200µm from the osteotomy)

void Create_temporal_FEM(int cell_1[N][N],int cell_2[N][N], int direction_2[N][N], int iter, int migration_dir[N][N],int& number_vessels_temp, int tip_x_temp[(N-1)*20], int tip_y_temp[(N-1)*20], double X_temp[N*N],double Y_temp[N*N], int previous_dir_temp [(N-1)*20]);
void Initialize_model(int cell_matrix[N][N], int vessel_x[(N-1)*20][N-1], int vessel_y[(N-1)*20][N-1], int force_dir[N][N], int fibro_number, int& number_vessels_init, int tip_vessel_x [(N-1)*20], int tip_vessel_y [(N-1)*20], int previous_dir [(N-1)*20], int length [(N-1)*20], double X[N*N], double Y[N*N]);
void Read_displacements(int cells[N][N], int dip_dir[N][N], double deformation[N][N], int t,int model_type);
void Update_model(int cell_matrix_1[N][N], int dipole_dir_1[N][N], double cell_def_1[N][N], int dipole_dir_2[N][N], double cell_def_2[N][N],int iter, int migration[N][N]);
void Proliferation_apoptosis(int cell_matrix[N][N], int dipole[N][N],int iter, int cell_number, int& number_vessels_prol, int tip_x_prol[(N-1)*20], int tip_y_prol[(N-1)*20], double X[N*N], double Y[N*N],int previous_dir_proli [(N-1)*20]);
void Write_batch_file(int iter, int model_type);

// Angiogenesis
void Blood_vessels(int cells_vessels[N][N],int vessels_x[(N-1)*20][N-1],int vessels_y[(N-1)*20][N-1], int& num_vessel, int tip_x[(N-1)*20], int tip_y[(N-1)*20], int previous_direction[(N-1)*20], int v_length[(N-1)*20], int iter);
int Direction_strains(int tip_x, int tip_y,double E11[N*N], double E22[N*N], double E12[N*N],float k[N*N]);
int Branching(int length);
float count_ec(int j, int i, int cells[N][N], int iter);
//
void Direction_growth_1(int cells_dir1[N][N],int j_dir1, int i_dir1, int& new_branch_j_dir1,int& new_branch_i_dir1,int& length_increase1,int& visual_i_dir1,int& visual_j_dir1);
void Direction_growth_2(int cells_dir2[N][N],int j_dir2, int i_dir2, int& new_branch_j_dir2,int& new_branch_i_dir2,int& length_increase2,int& visual_i_dir2,int& visual_j_dir2);
void Direction_growth_3(int cells_dir3[N][N],int j_dir3, int i_dir3, int& new_branch_j_dir3,int& new_branch_i_dir3,int& length_increase3,int& visual_i_dir3,int& visual_j_dir3);
void Direction_growth_4(int cells_dir4[N][N],int j_dir4, int i_dir4, int& new_branch_j_dir4,int& new_branch_i_dir4,int& length_increase4,int& visual_i_dir4,int& visual_j_dir4);
void Direction_growth_5(int cells_dir5[N][N],int j_dir5, int i_dir5, int& new_branch_j_dir5,int& new_branch_i_dir5,int& length_increase5,int& visual_i_dir5,int& visual_j_dir5);
void Direction_growth_6(int cells_dir6[N][N],int j_dir6, int i_dir6, int& new_branch_j_dir6,int& new_branch_i_dir6,int& length_increase6,int& visual_i_dir6,int& visual_j_dir6);
void Direction_growth_7(int cells_dir7[N][N],int j_dir7, int i_dir7, int& new_branch_j_dir7,int& new_branch_i_dir7,int& length_increase7,int& visual_i_dir7,int& visual_j_dir7);
void Direction_growth_8(int cells_dir8[N][N],int j_dir8, int i_dir8, int& new_branch_j_dir8,int& new_branch_i_dir8,int& length_increase8,int& visual_i_dir8,int& visual_j_dir8);
//
void Branch_dir1(int cells_branch1[N][N],int j_branch1, int i_branch1, int& previous_direction_branch1, int& new_j_branch1, int& new_i_branch1, int& length_branch1,int& visual_i_branch1,int& visual_j_branch1);
void Branch_dir2(int cells_branch2[N][N],int j_branch2, int i_branch2, int& previous_direction_branch2, int& new_j_branch2, int& new_i_branch2, int& length_branch2,int& visual_i_branch2,int& visual_j_branch2);
void Branch_dir3(int cells_branch3[N][N],int j_branch3, int i_branch3, int& previous_direction_branch3, int& new_j_branch3, int& new_i_branch3, int& length_branch3,int& visual_i_branch3,int& visual_j_branch3);
void Branch_dir4(int cells_branch4[N][N],int j_branch4, int i_branch4, int& previous_direction_branch4, int& new_j_branch4, int& new_i_branch4, int& length_branch4,int& visual_i_branch4,int& visual_j_branch4);
void Branch_dir5(int cells_branch5[N][N],int j_branch5, int i_branch5, int& previous_direction_branch5, int& new_j_branch5, int& new_i_branch5, int& length_branch5,int& visual_i_branch5,int& visual_j_branch5);
void Branch_dir6(int cells_branch6[N][N],int j_branch6, int i_branch6, int& previous_direction_branch6, int& new_j_branch6, int& new_i_branch6, int& length_branch6,int& visual_i_branch6,int& visual_j_branch6);
void Branch_dir7(int cells_branch7[N][N],int j_branch7, int i_branch7, int& previous_direction_branch7, int& new_j_branch7, int& new_i_branch7, int& length_branch7,int& visual_i_branch7,int& visual_j_branch7);
void Branch_dir8(int cells_branch8[N][N],int j_branch8, int i_branch8, int& previous_direction_branch8, int& new_j_branch8, int& new_i_branch8, int& length_branch8,int& visual_i_branch8,int& visual_j_branch8);
//
void Write_results(int cell_matrix[N][N], int dipole[N][N],int t_write);
void Write_data_vessels(int time, int vessels_x[(N-1)*20][N-1],int vessels_y[(N-1)*20][N-1], int& num_vess);
void Read_cell_matrix(int cell_matrix[N][N], int dipole_matrix[N][N], int iteration);

     

    




